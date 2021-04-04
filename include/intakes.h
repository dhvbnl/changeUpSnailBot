#include "vex.h"

//constants
const int tempLimitIntake = 55;
const double voltageConversationIntake = 8 + (1/3);

//complete functions
int intakeControl();

//setters
void setIntakeSpeed(int speed);

void setIntakeCreep();
void setIntakeLock();

//getters
int getIntakeSpeed();

int getLIntakeSpeed();
int getRIntakeSpeed();

int getLIntakeTemp();
int getRIntakeTemp();

//control 
void pauseIntake();
void playIntake();

void resetIntakes();

std::string tempinfoIntake();

