#include "vex.h"

//threads
thread drivetrainControl();

//setters
void setSpeed(int leftSpeed, int rightSpeed);

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
const char* tempInfo();
