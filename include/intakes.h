#include "vex.h"

//constants
const int tempLimitIntake = 55;

//threads
void intakeControl();

//setters
void setSpeedIntake(int speed);

//getters
int getIntakeSpeed();

int getLIntakeSpeed();
int getRIntakeSpeed();

int getLIntakeTemp();
int getRIntakeTemp();

//control 
std::string tempinfoIntake();
