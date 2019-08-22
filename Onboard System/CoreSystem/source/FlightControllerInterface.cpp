#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
//#include <wiringPiSPI.h>
#include <string>
#include <thread> // std::thread
#include <unistd.h>
#include <mutex>
#include <future>
#include <functional>

#include "ControllerInterface.hpp"

#include "common.hpp"

#include "Sensors/Sensors.hpp"
#include "Sensors/InertialMeasurement.hpp"
#include "Sensors/Location.hpp"

/* ------------------------------------------------------------------------------------------------------------------------ */
/* -----------------------------------------Some Pretty Definitions we may need-------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

uint8_t RC_DATA[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//using namespace std;
std::mutex mtx;
std::mutex failsafe;

std::thread *FailSafeThread;

bool FaultManaged = false;
bool FailSafeTrigger = false;

#if defined(ONBOARD_SPI_PROTOCOL) || defined(NRF24L01_SPI_PROTOCOL) || defined(I2C_PROTOCOL)
struct ControlPackets
{
    unsigned char magic;
    unsigned char throttle;
    unsigned char pitch;
    unsigned char roll;
    unsigned char yaw;
    unsigned char aux1;
    unsigned char aux2;
    unsigned char switches;
    //unsigned char random[9];
    unsigned char checksum;
};

struct ResponsePackets
{
    unsigned char magic;
    unsigned char alt;
    unsigned char pitch;
    unsigned char roll;
    unsigned char yaw;
    unsigned char lat;
    unsigned char lon;
    unsigned char heading;
    //unsigned char random[9];
    unsigned char checksum;
};

struct CommandPackets
{
    uint8_t magic;
    uint8_t value;
    uint8_t channel;
    uint8_t checksum;
};

ControlPackets defCp;
ControlPackets oldDefCp;
ResponsePackets rff;
ControlPackets *pp = &defCp;
ControlPackets *ppold = &oldDefCp;
#else

#endif

timespec *t100n = new timespec;
timespec *t1000n = new timespec;
timespec *t10000n = new timespec;
timespec *t100000n = new timespec;

uint8_t IMU_Raw[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
uint8_t PID_Raw[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
uint8_t Current_PID_Var = 0;

struct MSP_Packet
{
    char *buf;
    int size;

    MSP_Packet(char *buf, int sz) : buf(buf), size(sz){};
};

MSP_Packet MSP_Agent(char *buf, int size);

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ----------------------------------------------Some Preprocessor Checkups------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined(MODE_AIRSIM)
#undef MSP_SERIAL_CLI_MONITOR // We don't use MSP with airsim; instead we may use mavlink
#undef MSP_Serial_PROTOCOL
#endif

#if !defined(MODE_AIRSIM) && !defined(MODE_MAVLINK_SIM) && !defined(MODE_DEBUG_NO_FC)
#define MODE_REALDRONE
#endif

#if defined(MODE_DEBUG_FC) || (!defined(ONBOARD_SPI_PROTOCOL) && !defined(NRF24L01_SPI_PROTOCOL) && !defined(I2C_PROTOCOL) && !defined(MSP_Serial_PROTOCOL) && !defined(MODE_AIRSIM))
#define FAKE_PROTOCOL
#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ---------------------------------------------For SPI Based Communication------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined(ONBOARD_SPI_PROTOCOL)

#ifndef MODE_DEBUG_NO_FC
#include "LowLevel/SPIdrivers.h"
#include "wiringPiSPI.h"
#endif

int fd;

#define TIMEOUT_VAL 20

/*
        The new handshake mechanism works like this ->
        Send a special msg, then wait for an amount of time for it to arrive. 
        if it arrives, validate, else print handshake failed or timed out
    */

unsigned char checksum(unsigned char *buf, int len)
{
    unsigned char tt = 0;
    for (int i = 0; i < len - 1; i++)
    {
        tt += buf[i];
    }
    return tt;
}

int SPI_handshake()
{
    int ht = REQ2_SIGNAL;
#ifndef MODE_DEBUG_NO_FC
    SPI_ReadWrite((int)fd, (uintptr_t)&ht, (uintptr_t)&ht, (size_t)1); //wiringPiSPIDataRW(0, (unsigned char*)&ht, 1);
#endif
    //nanosleep(t100000n, NULL);
    return 0;
}

int IssueCommand()
{
    if (!SPI_handshake())
    {
        uint8_t *ht = ((uint8_t *)pp);
        uint8_t *hr = ((uint8_t *)&rff);
        pp->checksum = checksum(ht, sizeof(ControlPackets));
        uint8_t tb = 0;
#ifndef MODE_DEBUG_NO_FC
        SPI_ReadWrite((int)fd, (uintptr_t)ht, (uintptr_t)hr, (size_t)sizeof(ControlPackets));
#endif
        std::cout << "Successfully Issued Command\n";
        return 0;
    }
    return 1;
}

void Channel_Updater(int threadid)
{
    std::cout << "\nSPI Updater Initialized...";
    uint8_t *tbo = (uint8_t *)ppold;
    uint8_t *tb = (uint8_t *)pp;
    while (1)
    {
        /*
                Well, Basically, Firstly, check if the new data that we are gonna send is identical to the older, we don't need to send anything!
            */
        for (int i = 0; i < sizeof(ControlPackets); i++)
        {
            if (tb[i] != tbo[i])
                goto update;
        }
        goto skip;
    update:
        memcpy((void *)tbo, (void *)tb, sizeof(ControlPackets));
        if (IssueCommand())
        {
            std::cout << "Couldn't Issue the command\n";
        }
    skip:
        nanosleep(t10000n, NULL);
    }
}

void Raw_Init(int argc, char *argv[])
{
#ifndef MODE_DEBUG_NO_FC
    //fd = SPI_init("/dev/spidev0.0");
    fd = wiringPiSPISetup(0, 500000);
#endif
}

static volatile void sendCommand(uint8_t val, uint8_t channel)
{
    // First send the value, then the channel, then a dummy and check the returned checksum.
    // if its good, okay otherwise repeat.
    mtx.lock();
    uint8_t *bv = new uint8_t;
    uint8_t *bc = new uint8_t;
    int flg1 = 0, flg2 = 0;
    uint8_t *tmp1 = new uint8_t;
    uint8_t *tmp2 = new uint8_t;
    int counter = 0;
    uint8_t *t1 = new uint8_t;
    uint8_t *t2 = new uint8_t;
back:
    printf("\n[Attempting send %d to %d <%d, %d>", val, channel, flg1, flg2);
    *bv = val;
    *bc = channel;

#ifndef MODE_DEBUG_NO_FC
    wiringPiSPIDataRW(0, bv, 1); // Send the value
#endif
    nanosleep(t100000n, NULL);

#ifndef MODE_DEBUG_NO_FC
    wiringPiSPIDataRW(0, bc, 1); // Send the channel, recieve the value
#endif
    nanosleep(t100000n, NULL);

    if (*bc == (val ^ 0xff) || flg1)
    {
        printf(" [correct val %d %d, %d] ", *bc, (val ^ 0xff), flg1);
        *tmp1 = val;
        *t1 = (*bc ^ 0xff);
        flg1 = 1;
    }
    else
    {
        printf(" [false val %d %d, %d] ", *bc, (val ^ 0xff), flg1);
        *tmp1 = 0xff;
        flg1 = 0;
    }

#ifndef MODE_DEBUG_NO_FC
    wiringPiSPIDataRW(0, tmp1, 1); // Send confirmation of reception, recieve channel
#endif
    nanosleep(t100000n, NULL);

    if (*tmp1 == (channel ^ 0xff) || flg2)
    {
        printf(" [correct channel %d %d, %d] ", *tmp1, (channel ^ 0xff), flg2);
        *tmp2 = channel;
        *t2 = (*tmp1 ^ 0xff);
        flg2 = 1;
    }
    else
    {
        printf(" [false channel %d %d, %d] ", *tmp1, (channel ^ 0xff), flg2);
        *tmp2 = 0xff;
        flg2 = 0;
    }

#ifndef MODE_DEBUG_NO_FC
    wiringPiSPIDataRW(0, tmp2, 1); // Send confirmation of reception, recieve something
#endif
    nanosleep(t100000n, NULL);

    /*
            If the value was wrong but channel was right, send the value only;
            and vice versa
        */
    if (flg1 && flg2)
    {
        printf("\tSuccess!, %d, %d", *t1, *t2);
        delete bv;
        delete bc;
        delete tmp1;
        delete tmp2;
        mtx.unlock();
        return;
    }
    else
    {
        printf("\t[Retrying]");
        goto back;
    }
}
#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ---------------------------------------------For I2C Based Communication------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined I2C_PROTOCOL
#include "LowLevel/I2Cdrivers.h"

int IssueCommand()
{
}

void Raw_Init(int argc, char *argv[])
{
}

void sendCommand(uint8_t val, uint8_t channel)
{
}

#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ---------------------------------------------For Nrf24L01 Communication------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined NRF24L01_SPI_PROTOCOL

#include "RadioDrivers.cpp"

int IssueCommand()
{
    NRF24_Send((uintptr_t)pp, (uintptr_t)&rff, sizeof(ControlPackets));
}

void Raw_Init(int argc, char *argv[])
{
}

static volatile void sendCommand(uint8_t val, uint8_t channel)
{
}

#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* -----------------------------------------For MSP Based Serial directly to FC-------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined MSP_Serial_PROTOCOL

#include "LowLevel/msp/MSP.hpp"
#include "LowLevel/msp/msg_print.hpp"
#include "LowLevel/msp/msp_id.hpp"
#include "LowLevel/msp/FlightController.hpp"

fcu::FlightController *FlController;

int IssueCommand()
{
    return 0;
}

uint16_t rcExpand(uint8_t val) // Basically map val from 0, 255 to 1000 to 2000
{
    uint16_t aa = 1000 + uint16_t(int(3.938 * double(val))); // The formula to map ranges
    if (aa > 2000)
        aa = 2000;

    return aa;
}

void Channel_Updater(int threadId)
{
    // Basicaly, make sure to update every 5 seconds to convey the FC that everything is fine.
    while (1)
    {
        try
        {
            mtx.lock();
            FlController->setRc(rcExpand(RC_DATA[ROLL]), rcExpand(RC_DATA[PITCH]), rcExpand(RC_DATA[YAW]), rcExpand(RC_DATA[THROTTLE]), rcExpand(RC_DATA[AUX1]), rcExpand(RC_DATA[AUX2]), rcExpand(RC_DATA[AUX3]), rcExpand(RC_DATA[AUX4]));
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        catch (const std::future_error &e)
        {
            std::cout << "<Channel_Updater>Caught a future_error with code \"" << e.code()
                      << "\"\nMessage: \"" << e.what() << "\"\n";
            mtx.unlock();
        }
        catch (std::exception &e)
        {
            std::cout << "Error in Channel Updater " << e.what();
            mtx.unlock();
        }
    }
}

void Raw_Init(int argc, char *argv[])
{
    const std::string device = (argc > 1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const size_t baudrate = (argc > 2) ? std::stoul(argv[2]) : 115200;
    /*
        msp::MSP msp(device, baudrate);
        msp.setWait(1);
    */
    std::cout << "\n\tAttempting to connect to the Flight Controller...\n";
    std::chrono::high_resolution_clock::time_point start, end;
    FlController = new fcu::FlightController(device, baudrate);

    // wait until connection is established
    // get unique box IDs
    start = std::chrono::high_resolution_clock::now();
    FlController->initialise();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "FC connected, ready after: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    // on cleanflight, we need to enable the "RX_MSP" feature
    if (FlController->isFirmwareCleanflight())
    {
        std::cout << "\n\n\tCleanFlight/BetaFlight FC Identified and Successfully Connected\n\n";
    }
    else if (FlController->isFirmwareMultiWii())
    {
        std::cout << "\n\n\tMultiWii FC Identified and Successfully Connected\n\n";
    }

    std::cout << "Armed? " << FlController->isArmed() << std::endl;

    // disarm the FC
    std::cout << "Disarming..." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    FlController->disarm_block();
    end = std::chrono::high_resolution_clock::now();

    if (!FlController->isArmed())
    {
        std::cout << "disarmed after: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    }
}

void sendCommand(uint8_t val, uint8_t channel)
{
    FlController->setRc(rcExpand(RC_DATA[ROLL]), rcExpand(RC_DATA[PITCH]), rcExpand(RC_DATA[YAW]), rcExpand(RC_DATA[THROTTLE]), rcExpand(RC_DATA[AUX1]), rcExpand(RC_DATA[AUX2]), rcExpand(RC_DATA[AUX3]), rcExpand(RC_DATA[AUX4]));
}

#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* -------------------------------------For AirSim Simple FC, through provided APIs---------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined MODE_AIRSIM

#if defined(AIRSIM_MODE_SOCKET)

#elif defined(AIRSIM_MODE_API)
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "rpc/server.h"

#define TIMESLICE 0.001

msr::airlib::MultirotorRpcLibClient *client;

/*

def valMap(i, imin, imax, omin, omax):
    aa = omin + (((omax - omin) / (imax - imin)) * (i - imin))  # The formula to map ranges
    return aa
*/

inline float rcShrink(uint8_t val, float omin = -1.0, float omax = 1.0)
{
    float aa = omin + (((omax - omin) / (255.0)) * int(val));
    return aa;
}

int IssueCommand(int threadId)
{
    //NRF24_Send((uintptr_t)pp, (uintptr_t)&rff, sizeof(ControlPackets));
    return 0;
}

quaternion_t AIRSIM_oritentation;
GeoPoint_t AIRSIM_location;
vector3D_t AIRSIM_velocity;
vector3D_t AIRSIM_euleroritentation;

void Sensors_Updater()
{
    while (1)
    {
        try 
        {
            //mtx.lock();
            auto orien = client->getMultirotorState().getOrientation();
            AIRSIM_oritentation.set(orien.w(), orien.x(), orien.y(), orien.z());
            //auto velocity = client->getMultirotorState().kinematics_estimated.twist.linear;
            //AIRSIM_velocity.set(velocity.x(), velocity.y(), velocity.z());
            auto location = client->getMultirotorState().getPosition();
            AIRSIM_location.set(location.y(), location.x(), -location.z());
            auto euler = eulerFromQuaternion(AIRSIM_oritentation);
            AIRSIM_euleroritentation.set(euler.x, euler.y, euler.z);
            //mtx.unlock();
            //std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        catch (const std::future_error &e)
        {
            std::cout << "<Sensor_Updater>Caught a future_error with code \"" << e.code()
                      << "\"\nMessage: \"" << e.what() << "\"\n";
            //mtx.unlock();
        }
        catch (std::exception &e)
        {
            std::cout << "Error in CLI Monitor " << e.what();
            //mtx.unlock();
        }
    }
}

void Channel_Updater(int threadid)
{
    std::thread sensors(Sensors_Updater);
    while (1)
    {
        try
        {
            //mtx.lock();
            client->moveByAngleThrottleAsync(rcShrink(255 - RC_DATA[PITCH]), rcShrink(RC_DATA[ROLL]), rcShrink(RC_DATA[THROTTLE], 0, 5), rcShrink(RC_DATA[YAW], -10.0, 10.0), TIMESLICE);
            //mtx.unlock();

            std::this_thread::sleep_for(std::chrono::microseconds(int(TIMESLICE * 1000.0 * 1000.0)));
        }
        catch (const std::future_error &e)
        {
            std::cout << "<Channel_Updater>Caught a future_error with code \"" << e.code()
                      << "\"\nMessage: \"" << e.what() << "\"\n";
            //mtx.unlock();
        }
        catch (std::exception &e)
        {
            std::cout << "Error in CLI Monitor " << e.what();
            //mtx.unlock();
        }
    }
    sensors.join();
}

void Raw_Init(int argc, const char *argv[])
{
    const std::string ip = (argc > 1) ? std::string(argv[1]) : "localhost";
    const uint16_t port = (argc > 2) ? std::stoi(argv[2]) : 41451;
    //rpc::server srv(8080);

    //cout << "Press Enter to enable API control" << endl; cin.get();
    client = new msr::airlib::MultirotorRpcLibClient(ip, port);
    printf("\nEnabling AirSim API Control");
    client->enableApiControl(true);

    //cout << "Press Enter to arm the drone" << endl; cin.get();
    printf("\nArming the multirotor");
    client->armDisarm(true);

    //cout << "Press Enter to takeoff" << endl; cin.get();
    //client->takeoffAsync(5); //*/
    printf("\nInitialization complete");
    ControllerInterface::MainIMU = new AirSim_IMU_t(client);
    ControllerInterface::MainLocator = new AirSim_Locator_t(client);
}

static volatile void sendCommand(uint8_t val, uint8_t channel)
{
    //client.moveByAngleThrottleAsync(rcShrink(RC_DATA[PITCH]), rcShrink(RC_DATA[ROLL]), rcShrink(RC_DATA[THROTTLE], 0, 10), rcShrink(RC_DATA[YAW], -6, 6), 10);
}

#endif
#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* --------------------------------------For Testing without FC, on development PC----------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined(MODE_DEBUG_NO_FC) || defined(FAKE_PROTOCOL)

int IssueCommand()
{
    return 0;
}

void Channel_Updater(int threadid)
{
    while (1)
        ;
}

void Raw_Init(int argc, char *argv[])
{
}

static volatile void sendCommand(uint8_t val, uint8_t channel)
{
    printf("\n[%d ------> %d", val, channel);
}

#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* -----------------------------------------------RX Definitions ends here------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------Protocol Specific Data Gathering---------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined(MSP_Serial_PROTOCOL)
#endif

#if defined(MODE_AIRSIM)
#endif

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ---------------------------------------------MSP Data stream forwarding------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#if defined(MSP_SERIAL_FORWARDING)

//#include "LowLevel/Serial.cpp"

#include "LowLevel/msp/MSP.hpp"
#include "LowLevel/msp/msg_print.hpp"
#include "LowLevel/msp/msp_id.hpp"
#include "LowLevel/msp/FlightController.hpp"

bolatile char *bb = new char[4096];

volatile msp::MSP *msp_agent;

MSP_Packet MSP_Agent(char *buf, int size)
{
    try
    {
        int j = 0;
        std::vector<uint8_t> vec((uint8_t *)buf, (uint8_t *)(buf + size));
        /*for (int i = 0; i < size; i++)
        {
            if (buf[i] == '$' && buf[i + 1] == 'M') // MSP Header
            {
                //buf[i+2] contains the direction
                //buf[i+3] contains the size
            }
            printf("[%x]", vec[i]);
        } //*/
        printf("\n Sending...");
        while (msp_agent->write(vec) != 1)
            ;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (msp_agent->hasData() < 1)
        {
            exit(0);
            throw "No Data";
        }

        while (char(msp_agent->read()) != '$')
            ;

        printf("\n Got Data!");

        uint8_t *rdat = (uint8_t *)bb; //malloc(1024); //sz + 1);
        rdat[0] = '$';
        rdat[1] = (uint8_t)msp_agent->read(); //hdr;
        rdat[2] = (uint8_t)msp_agent->read(); //com_state;
        rdat[3] = (uint8_t)msp_agent->read(); //data_size;
        rdat[4] = (uint8_t)msp_agent->read(); //id;

        int i;
        int sz = rdat[3] + 6;
        for (i = 5; i < rdat[3] + 5; i++)
        {
            rdat[i] = (uint8_t)msp_agent->read(); //data[i - 5];
            //printf("{{%d}}", rdat[i]);
        }

        rdat[sz - 1] = (uint8_t)msp_agent->read(); //rcv_crc;
        printf("Size: %d", sz);

        /*for (int j = 0; j < sz; j++)
        {
            printf("<<%x>>", rdat[j]);
        }*/

        return MSP_Packet((char *)rdat, sz);
    }
    catch (std::exception &e)
    {
        printf("\n Some Error");
        //while(1);
        return MSP_Packet(NULL, 0);
    }
}

//std::vector<SerialPort *> defaultPorts;

void Port_Forwarding_Init(int argc, char *argv[])
{
    const std::string device = (argc > 1) ? std::string(argv[1]) : "/dev/ttyUSB0";
    const size_t baudrate = (argc > 2) ? std::stoul(argv[2]) : 115200;
    std::cout << "Opening Port " << device << std::endl;
    msp_agent = new msp::MSP(device, baudrate);
    //defaultPorts.push_back(new SerialPort(device.c_str(), baudrate));
    /*char* buf = new char[4096];
    for(int i = 0; i < 1000; i++)
    {
       // printf(".");
        std::cout<<ReadFromPort(0, buf, 100)<<std::endl;
        printf(buf);
    }
    while(1);*/
}
/*
std::mutex serial_mtx;

int WriteToPort(int portnum, char *buff, int size)
{
    //serial_mtx.lock();
    int a = defaultPorts[portnum]->Write(buff, size);
    //printf("..");
    //serial_mtx.unlock();
    return a;
}

int ReadFromPort(int portnum, char *buff, int size)
{
    //serial_mtx.lock();
    int a = defaultPorts[portnum]->Read(buff, size);
    //serial_mtx.unlock();
    return a;
}*/

#endif