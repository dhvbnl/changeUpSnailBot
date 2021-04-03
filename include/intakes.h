#include "vex.h"

//constants
const int tempLimitIntake = 55;
const double voltageConversationIntake = 8 + (1/3);

//complete functions
int intakeControl();

//setters
void setSpeedIntake(int speed);

//getters
int getIntakeSpeed();

int getLIntakeSpeed();
int getRIntakeSpeed();

int getLIntakeEfficiency();
int getRIntakeEffiency();
int getLIntakePower();
int getRIntakePower();
double getLIntakeTorque();
double getRIntakeTorque();

int getLIntakeTemp();
int getRIntakeTemp();

bool getLIntakeStress();
bool getRIntakeStress();

//control 
std::string tempinfoIntake();
void pauseIntake();
void playIntake();
