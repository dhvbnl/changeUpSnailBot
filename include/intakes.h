#include "vex.h"

//constants
const int tempLimit = 55;

//threads
thread intakeControl();

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
