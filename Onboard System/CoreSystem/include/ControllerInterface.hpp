#pragma once

#ifndef CONTROLLER_INTERFACE_H
#define CONTROLLER_INTERFACE_H

#include "stdint.h"
#include "string"
#include "iostream"
#include "vector"
#include "algorithm"
#include "mutex"
#include "thread"

#include "common.hpp"

#include "Sensors/Sensors.hpp"
#include "Sensors/InertialMeasurement.hpp"
#include "Sensors/Location.hpp"

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ---------------------------------------------------Some Definitions----------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------------ */

#define CP_MAGIC 110

uint8_t checksum(uint8_t *buf, int len);

#define CPACKET_MAGIC 110
#define REQ_SIGNAL 251
#define REQ2_SIGNAL 101
#define ACCEPT_SIGNAL 252
#define RPACKET_MAGIC 120
#define FALSE_PACKET 145
#define ACK_GOT_PACKET 250

#define THROTTLE 0
#define PITCH 1
#define ROLL 2
#define YAW 3
#define AUX1 4
#define AUX2 5
#define AUX3 6
#define AUX4 7

#define RC_X_MOTION 8
#define RC_Y_MOTION 9

#define HEADING_YAW_P 0.8
#define HEADING_YAW_I 1
#define HEADING_YAW_D 200
#define HEADING_YAW_DAMPING 1
#define HEADING_YAW_DAMPING_2 0.8


namespace ControllerInterface
{
static InertialMeasurement_t *MainIMU;
static GlobalLocator_t *MainLocator;
static GlobalState_t *MainState;

static uint8_t RC_MASTER_DATA[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int WriteToPort(int portnum, char *buff, int size);
int ReadFromPort(int portnum, char *buff, int size);
void setThrottle(int throttle);
void setPitch(int pitch);
void setRoll(int roll);
void setYaw(int yaw);
void setAux1(int val);
void setAux2(int val);
void setAux3(int val);
void setAux4(int val);

void setDistance(float val);

uint8_t getGyro(int axis);
uint8_t getAcc(int axis);
uint8_t getMag(int axis);
uint8_t getPID_P(int axis);
uint8_t getPID_I(int axis);
uint8_t getPID_D(int axis);
uint8_t getArmStatus(int block);

float getYaw();   // Gives in Radians, Absolute for drone wrt world
float getRoll();  // Gives in Radians, Absolute for drone wrt world
float getPitch(); // Gives in Radians, Absolute for drone wrt world

float getYawDegrees();
float getRollDegrees();
float getPitchDegrees();
float getHeadingDegrees(); // Gives in Degrees
float getHeading();

float get_X_Coordinate();
float get_Y_Coordinate();
float getAltitude();

float get_X_VelocityRel();
float get_Y_VelocityRel();
float get_Z_VelocityRel();
float get_X_VelocityAbs();
float get_Y_VelocityAbs();
float get_Z_VelocityAbs();

vector3D_t getVelocityAbs();
vector3D_t getVelocityRel();

GeoPoint_t getLocation(); // CHANGE THIS
vector3D_t getVelocity();
image_t getCameraView();
data_imu_t getIMU();
DroneState_t getCompleteState();

quaternion_t getOrientationQuaternion();
vector3D_t getOrientation(); // Returns Euler angle orientation

float getDesiredVelocity();
float getCurrentTargetDistance();
void HeadlessMoveTowardsTarget(float val);

int setAutoYaw(float heading);
int setAutoRoll(float heading);
int setAutoPitch(float heading);

int setHeading(float heading);
int testHeading(int test);

int setVelocity(vector3D_t val);
void set_Y_Motion(int val);
void set_X_Motion(int val);
void set_X_MotionAbs(int val);
void set_Y_MotionAbs(int val);

int setAltitude(float altitude);

void takeOff(float altitude = 5);

int setVelocity(vector3D_t val);
int setPosition(GeoPoint_t val);
/*
    High Level APIs 
*/

int AutoNavigate(GeoPoint_t destination, GeoPoint_t start);
int gotoLocation(float x, float y, float z);
int addWaypoint(float x, float y, float z);

int returnToHome();

int toggleFeedbackControllers(char type);

int launch_ActuationControllers();
void ResumeHandler();
void FaultHandler();

int ControllerInterface_init(int argc, const char *argv[]);
} // namespace ControllerInterface

volatile static std::thread *chnl_refresh;
volatile static std::thread *keyboard_handler;
volatile static std::thread *chnl_update;

#endif