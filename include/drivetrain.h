#include "vex.h"

//constants
const int tempLimitDrive = 55;
const double quadraticA = 0.01;

//complete functions
void drivetrainControl();

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
