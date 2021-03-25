#include "vex.h"

//constants
const int tempLimit = 55;
const double quadraticA = 0.01;

//threads
thread drivetrainControl();

//setters
void setSpeedDrivetrain(int leftSpeed, int rightSpeed);

//getters
int getLeftSpeedIn();
int getRightSpeedIn();

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
