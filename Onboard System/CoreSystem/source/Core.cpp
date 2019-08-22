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
#include <math.h>

#include "Sensors/Sensors.hpp"
#include "Sensors/InertialMeasurement.hpp"
#include "Sensors/Location.hpp"

#include "FlightControllerInterface.cpp"
#include "FeedbackControl.cpp"
#include "AutoNavigation.cpp"
#include "UserInterface.cpp"

namespace ControllerInterface
{
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ----------------------------------------------General APIs for Control-------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

/******************************************************************************************/
/******************************* A Low-High level Get APIs ********************************/
/******************************************************************************************/

uint8_t getGyro(int axis)
{
    //return IMU_Raw[0][axis];
    try
    {
        return IMU_Raw[0][axis];
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getGyro>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

uint8_t getAcc(int axis)
{
    //return IMU_Raw[1][axis];
    try
    {
        return IMU_Raw[1][axis];
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getAcc>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

uint8_t getMag(int axis)
{
    //return IMU_Raw[2][axis];
    try
    {
        return IMU_Raw[2][axis];
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getMag>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

uint8_t getPID_P(int axis)
{
    //return PID_Raw[0][axis];
    try
    {
        return PID_Raw[0][axis];
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getGyro>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

uint8_t getPID_I(int axis)
{
    //return PID_Raw[1][axis];
    try
    {
        return PID_Raw[1][axis];
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getPID_I>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

uint8_t getPID_D(int axis)
{
    //return PID_Raw[2][axis];
    try
    {
        return PID_Raw[2][axis];
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getPID_D>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

uint8_t getArmStatus(int block)
{
    return 0;
}

float getYaw() // Gives in Radians
{
    //   return MainState->getYaw();
    try
    {
        return MainState->getYaw();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getYaw>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getRoll() // Gives in Radians
{
    //return MainState->getRoll();
    try
    {

        return MainState->getRoll();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getRoll>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getPitch() // Gives in Radians
{
    //return MainState->getPitch();
    try
    {
        return MainState->getPitch();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getPitch>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getYawDegrees()
{
    //return MainState->getYawDegrees();
    try
    {
        return MainState->getYawDegrees();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getYawDegrees>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getRollDegrees()
{
    //return MainState->getRollDegrees();
    try
    {
        return MainState->getRollDegrees();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getRollDegrees>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getPitchDegrees()
{
    //return MainState->getPitchDegrees();
    try
    {
        return MainState->getPitchDegrees();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getPitchDegrees>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float get_X_Coordinate()
{
    //return MainState->get_X_Coordinate();
    try
    {
        return MainState->get_X_Coordinate();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<get_X_Coordinate>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float get_Y_Coordinate()
{
    //return MainState->get_Y_Coordinate();
    try
    {
        return MainState->get_Y_Coordinate();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<get_Y_Coordinate>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float get_X_VelocityRel()
{
    try
    {
        return MainState->get_X_VelocityRel();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<get_X_VelocityRel>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost get_X_VelocityRel loop!" << e.what();
    }
    return 0;
}

float get_Y_VelocityRel()
{
    try
    {
        return MainState->get_Y_VelocityRel();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<get_X_VelocityRel>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost get_X_VelocityRel loop!" << e.what();
    }
    return 0;
}

float get_Z_VelocityRel()
{
    try
    {
        return MainState->get_Z_VelocityRel();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<get_Z_VelocityRel>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost get_Z_VelocityRel loop!" << e.what();
    }
    return 0;
}

float get_X_VelocityAbs()
{
    return MainState->get_X_VelocityAbs();
}

float get_Y_VelocityAbs()
{
    return MainState->get_Y_VelocityAbs();
}

float get_Z_VelocityAbs()
{
    return MainState->get_Z_VelocityAbs();
}

float getAltitude()
{
    //return MainState->getAltitude();
    try
    {
        return MainState->getAltitude();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getAltitude>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getHeadingDegrees() // Gives in Degrees
{
    //return MainState->getHeadingDegrees();
    try
    {
        return MainState->getHeadingDegrees();
    }

    catch (const std::future_error &e)
    {
        std::cout << "<getHeadingDegrees>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

float getHeading()
{
    //return MainState->getHeading();
    try
    {
        return MainState->getHeading();
    }

    catch (const std::future_error &e)
    {
        std::cout << "<getHeading>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return 0;
}

/***********************************************************************************************/
/******************************* A Little Higher Level Get APIs ********************************/
/***********************************************************************************************/

vector3D_t getVelocityAbs()
{
    return MainState->getVelocityAbs();
}

vector3D_t getVelocityRel()
{
    try
    {
        return MainState->getVelocityRel();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getVelocityRel>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost getVelocityRel loop!" << e.what();
    }
    return vector3D_t(0,0,0);
}

vector3D_t getVelocity() // CHANGE THIS
{
    try
    {
        return MainState->getVelocity();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getVelocity>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost getVelocity loop!" << e.what();
    }
    return vector3D_t(0,0,0);
}

GeoPoint_t getLocation() // CHANGE THIS
{
    try
    {
        return MainState->getLocation();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<getLocation>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Y_Actuator loop!" << e.what();
    }
    return GeoPoint_t(0, 0, 0);
}

/******************************************************************************************/
/************************************ All the Set APIs ************************************/
/******************************************************************************************/

void setThrottle(int throttle)
{
    try
    {
        unsigned char t = (unsigned char)throttle;
        mtx.lock();
        RC_DATA[THROTTLE] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(throttle, 0);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setThrottle>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setPitch(int pitch)
{
    try
    {
        unsigned char t = (unsigned char)pitch;
        mtx.lock();
        RC_DATA[PITCH] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(pitch, 1);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setPitch>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setRoll(int roll)
{
    try
    {
        unsigned char t = (unsigned char)roll;
        mtx.lock();
        RC_DATA[ROLL] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(roll, 2);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setRoll>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setRoll(float roll)
{
    setRoll(int(roll));
}

void setPitch(float roll)
{
    setPitch(int(roll));
}

void setYaw(float roll)
{
    setYaw(int(roll));
}

void setYaw(int yaw)
{
    try
    {
        unsigned char t = (unsigned char)yaw;
        mtx.lock();
        RC_DATA[YAW] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(yaw, 3);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setYaw>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setAux1(int val)
{
    try
    {
        unsigned char t = (unsigned char)val;
        mtx.lock();
        // AUX1 to be used for PID Tuning, should set which
        RC_DATA[AUX1] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(val, 4);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAux1>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setAux2(int val)
{
    try
    {
        unsigned char t = (unsigned char)val;
        mtx.lock();
        RC_DATA[AUX2] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(val, 5);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAux2>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setAux3(int val)
{
    try
    {
        unsigned char t = (unsigned char)val;
        mtx.lock();
        RC_DATA[AUX3] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(val, 6);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAux3>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

void setAux4(int val)
{
    try
    {
        unsigned char t = (unsigned char)val;
        mtx.lock();
        RC_DATA[AUX4] = t;
#if defined(SYNCD_TRANSFER)
        sendCommand(val, 7);
#endif
        mtx.unlock();
        //IssueCommand();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAux4>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

/***********************************************************************************************/
/******************************* A Little Higher Level Set APIs ********************************/
/***********************************************************************************************/

int setHeading(float heading)
{
    try
    {
        return setAutoYaw(heading);
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setHeading>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost setHeading loop!" << e.what();
    }
    return 0;
}

int setVelocity(vector3D_t val)
{
    try
    {
        set_X_Velocity(val.x);
        set_Y_Velocity(val.y);
        setAltitude(val.z);
        return 0;
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setVelocity>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost setVelocity loop!" << e.what();
    }
    return 0;
}

int setAltitude(float altitude)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        FeedbackControl::Z_Actuator.actuationControllerlock->lock();
        FeedbackControl::Z_Actuator.setIntendedActuation(altitude);
        printf("\n>>%f", altitude);
        FeedbackControl::Z_Actuator.actuationControllerlock->unlock();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAltitude>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }

    return 0;
}

int setDestination(float x, float y, float z)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        setDestination(GeoPoint_t(x, y, z), true);
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setDestination>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }

    return 0;
}

int set_X_Velocity(float val)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        //FeedbackControl::X_Vrel_Actuator.actuationControllerlock->lock();
        //FeedbackControl::X_Vrel_Actuator.setIntendedActuation(val);
        //FeedbackControl::X_Vrel_Actuator.actuationControllerlock->unlock();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<set_X_Velocity>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost set_X_Velocity loop!" << e.what();
    }
    return 0;
}

int set_Y_Velocity(float val)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        //FeedbackControl::Y_Vrel_Actuator.actuationControllerlock->lock();
        //FeedbackControl::Y_Vrel_Actuator.setIntendedActuation(val);
        //FeedbackControl::Y_Vrel_Actuator.actuationControllerlock->unlock();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<set_Y_Velocity>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost set_Y_Velocity loop!" << e.what();
    }
    return 0;
}

int setAutoYaw(float heading)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        //printf("<<<%f>>>", heading);
        //FeedbackControl::YawActuator.actuationControllerlock->lock();
        FeedbackControl::YawActuator.setIntendedActuation(heading);
        //FeedbackControl::YawActuator.actuationControllerlock->unlock();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAutoYar>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }
    return 0;
}

int setAutoRoll(float heading)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        //FeedbackControl::RollActuator.actuationControllerlock->lock();
        FeedbackControl::RollActuator.setIntendedActuation(heading);
        //FeedbackControl::RollActuator.actuationControllerlock->unlock();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAutoRoll>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }
    return 0;
}

int setAutoPitch(float heading)
{
    try
    {
        if (IntentionOverride)
            return 1; // If the Feedback Controllers are overriden by the user manually, Do not attempt anything
        //FeedbackControl::PitchActuator.actuationControllerlock->lock();
        FeedbackControl::PitchActuator.setIntendedActuation(heading);
        //FeedbackControl::PitchActuator.actuationControllerlock->unlock();
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setAutoPitch>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }
    return 0;
}

/***********************************************************************************************/
/*********************************** Other Higher Level APIs ***********************************/
/***********************************************************************************************/

void takeOff(float altitude)
{
    setAltitude(altitude);
    //setDestination(0, 0, 20);
}

/*
    High Level APIs 
*/

int setHeadingToPoint(GeoPoint_t destination)
{
    return setGazeOn(destination);
}

void holdPosition(float x, float y, float z)
{
    setDestination(x, y, z);
    std::thread xposition(X_Position_Hold);
    std::thread yposition(Y_Position_Hold);
    xposition.join();
    yposition.join();
}

namespace {

GeoPoint_t lastDestination(0, 0, 0);

int autoNavPID(GeoPoint_t destination, GeoPoint_t start = lastDestination)
{
    try
    {
        printf("\n Got here!");
        AutoNavigation::addToPathQueue((AutoNavigation::makePath(start, destination)));
        lastDestination.set(destination);
    }
    catch (const std::future_error &e)
    {
        std::cout << "<autoNavPID>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }
    return 0;
}

}

int setDestination(GeoPoint_t position, bool start_now)
{
    try
    {
        printf("\n Got here!");
        if (start_now)
        {
            if(AutoNavigation::isNavigationInProgress())
            {
                autoNavPID(position);
            }
            else 
            {
                autoNavPID(position, getLocation());
            }
        }
            
    }
    catch (const std::future_error &e)
    {
        std::cout << "<setDestination>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Error in Outermost Z_Actuator loop!" << e.what();
    }
    return 0;
}

int gotoLocation(float x, float y, float z)
{
    printf("\nTrying to move to position x=%f y=%f z=%f", x, y, z);
    return autoNavPID(GeoPoint_t(x, y, z), getLocation());
}

int returnToHome()
{
    return 0;
}

/*
    Some other Mechanisms
*/

int unusedAPIhandler(std::vector<std::string> test)
{
    std::cout << "\nOops! Seems someone sent me some wrong code!";
    return 1;
}
int toggleFeedbackControllers(char type)
{
    try
    {
        printf("\nToggling Actuators...");
        fflush(stdout);
        if (IntentionOverride)
            IntentionOverride = false;
        else
            IntentionOverride = true;

        int j = 0;
        if (type == 'R')
            j = 0;
        else if (type == 'V')
            j = 1;
        else if (type == 'P')
            j = 2;
        else if (type == 'A')
        {
            /*if(IntentionOverride)
                IntentionOverride = false;
            else IntentionOverride = true;*/
            toggleFeedbackControllers('R');
            toggleFeedbackControllers('P');
            toggleFeedbackControllers('V');
        }
        else
            return 1;

        if (!FeedbackControl::FeedbackControllerStatus[j])
        {
            FeedbackControl::FeedbackControllerStatus[j] = true;
            printf("\tLocking...");
            for (int i = 0; i < 3; i++)
            {
                FeedbackControl::FeedbackControllers[j][i]->actuationControllerlock->lock();
            }
        }
        else
        {
            FeedbackControl::FeedbackControllerStatus[j] = false;
            printf("\tUnlocking...");
            for (int i = 0; i < 3; i++)
            {
                FeedbackControl::FeedbackControllers[j][i]->actuationControllerlock->unlock();
            }
        }
    }
    catch (const std::future_error &e)
    {
        std::cout << "<toggleFeedbackControllers>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Some Error in toggleFeedbackControllers!" << e.what();
        fflush(stdout);
    }
    printf("\nDone...");
    fflush(stdout);
    return 0;
}

void FailSafeMechanism()
{
    try
    {
        // Decrease the throttle at constant rate to land
        std::cout << "\nInitiating FailSafe...";
        setYaw(127);
        setRoll(127);
        setPitch(127);
        setYaw(127);
        int a = RC_DATA[THROTTLE];
        for (int i = a; i > 100; i--)
        {
            failsafe.lock();
            if (FaultManaged)
            {
                failsafe.unlock();
                return;
            }
            std::cout << "\nLowering the throttle to " << i;
            setThrottle(i);
            failsafe.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(FAILSAFE_LANDING_RATE));
        }
        for (int i = 0; i < 50; i++) // 2 Seconds
        {
            failsafe.lock();
            if (FaultManaged)
            {
                failsafe.unlock();
                return;
            }
            std::cout << "\nWaiting for connection while Landing... " << i;
            setThrottle(i);
            failsafe.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(FAILSAFE_LANDING_RATE));
        }
        // Disarm and send FailSafe!
        FailSafeTrigger = true;
        setAux1(51); // Trigger Failsafe
        std::cout << "\nFailSafe Locked!";
        mtx.lock(); // Grab the lock and don't release until the fault is fixed
    }
    catch (const std::future_error &e)
    {
        std::cout << "<FailSafeMechanism>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Some More Error 2!" << e.what();
    }
}

void ResumeHandler()
{
    //#if defined(MSP_Serial_PROTOCOL)
    try
    {
        failsafe.lock();
        FaultManaged = true;
        failsafe.unlock();
        FailSafeThread->join();
        if (FailSafeTrigger)
        {
            mtx.unlock();
            FailSafeTrigger = false;
        }
        std::cout << "Fault Resumed and Managed!\n";
    }
    catch (const std::future_error &e)
    {
        std::cout << "<ResumeHandler>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Some More Error 3!" << e.what();
    }
    //#endif
}

void FaultHandler()
{
    //#if defined(MSP_Serial_PROTOCOL)
    try
    {
        std::cout << "Fault Occured!\nTriggering FailSafe!!!";
        FaultManaged = false;
        FailSafeThread = new std::thread(FailSafeMechanism);
    }
    catch (const std::future_error &e)
    {
        std::cout << "<FaultHandler>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Some More Error 1!" << e.what();
    }
    //#endif
}

/*
    The Main Initializer Function
*/

int ControllerInterface_init(int argc, const char *argv[])
{
    try
    {
        printf("\n Initializing Flight Controller Interface...");
        //int fd = wiringPiSPISetup(0, SPI_IOC_WR_MAX_SPEED_HZ);//SPI_init("/dev/spidev0.0");
        Raw_Init(argc, argv);

        IntentionOverride = false;

        MainState = new GlobalState_t(MainLocator, MainIMU);

#if defined(MSP_SERIAL_FORWARDING)
        Port_Forwarding_Init(argc, argv);
#endif
        /*
        pp->magic = CP_MAGIC;
        pp->throttle = 0;
        pp->pitch = 127;
        pp->roll = 127;
        pp->yaw = 254;*/

        t100n->tv_sec = 0;
        t100n->tv_nsec = 100;
        t1000n->tv_sec = 0;
        t1000n->tv_nsec = 1000;
        t10000n->tv_sec = 0;
        t10000n->tv_nsec = 10000;
        t100000n->tv_sec = 0;
        t100000n->tv_nsec = 100000;

        //SPI_handshake();
        //IssueCommand();

        FeedbackControl::FeedbackControllers[0][0] = &FeedbackControl::YawActuator;
        FeedbackControl::FeedbackControllers[0][1] = &FeedbackControl::RollActuator;
        FeedbackControl::FeedbackControllers[0][2] = &FeedbackControl::PitchActuator;
        FeedbackControl::FeedbackControllerStatus[0] = false;

        /*FeedbackControl::FeedbackControllers[1][0] = &FeedbackControl::X_Vrel_Actuator;
        FeedbackControl::FeedbackControllers[1][1] = &FeedbackControl::Y_Vrel_Actuator;
        FeedbackControl::FeedbackControllers[1][2] = &FeedbackControl::Z_Vrel_Actuator;
        FeedbackControl::FeedbackControllerStatus[1] = false;

        FeedbackControl::FeedbackControllers[2][0] = &FeedbackControl::X_Actuator;
        FeedbackControl::FeedbackControllers[2][1] = &FeedbackControl::Y_Actuator;*/
        FeedbackControl::FeedbackControllers[2][2] = &FeedbackControl::Z_Actuator;
        FeedbackControl::FeedbackControllerStatus[2] = false;

        setAltitude(0);
        
#if defined(CLI_MONITOR)
        chnl_refresh = new std::thread(Channel_ViewRefresh, 0);
        for (int i = 0; i < 256; i++)
            KeyMap[i] = event_key_other;
        KeyMap['q'] = event_key_q; // Show RC Values
        KeyMap['w'] = event_key_w; // show PID Values
        KeyMap['e'] = event_key_e; // show MainState Data
        KeyMap['r'] = event_key_r; // show Wifi Strength
        KeyMap['A'] = event_key_A; // show Armed
        KeyMap['P'] = event_key_P; // show Position
        KeyMap['V'] = event_key_V; // show Velocity
        KeyMap['h'] = event_key_h; // set PIDs
        keyboard_handler = new std::thread(Keyboard_handler, 2);
#endif
#if defined(UPDATER_THREAD)
        chnl_update = new std::thread(Channel_Updater, 1);
#endif

        AutoNavigation::initialize_AutoNavigation();
        return 0;
    }
    catch (const std::future_error &e)
    {
        std::cout << "<ControllerInterface_init>Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        //mtx.unlock();
    }
}

} // namespace ControllerInterface