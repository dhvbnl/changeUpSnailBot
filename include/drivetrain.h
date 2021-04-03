#include "vex.h"

//constants
const int tempLimitDrive = 55;
const double quadraticA = 0.12;
const int powerRatioB = 1;
const double voltageConversationDrive = 8 + (1/3);

//complete functions
int drivetrainControl();

//setters
void setSpeedDrivetrain(int leftSpeed, int rightSpeed);

//getters
int getLeftSpeedInLinear();
int getRightSpeedInLinear();

int getLeftSpeedInSlew();
int getRightSpeedInSlew();

int getLFrontSpeed();
int getLBackSpeed();
int getRBackSpeed();
int getRFrontSpeed();

int getLFrontTemp();
int getLBackTemp();
int getRBackTemp();
int getRFrontTemp();

//control 
std::string tempInfoDrive();
