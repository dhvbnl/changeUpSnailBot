#include "intakes.h"
#include "controller.h"

bool intakeState = true;

int intakeControl() {
  while (true) {
    if(intakeState){
      setSpeedIntake(getIntakeSpeed());
    }
    wait(10,msec);
  }
}

//setters

// sets speed for both intakes based on one speed parameter
void setSpeedIntake(int speed) {
  lIntake.spin(fwd, speed, volt);
  rIntake.spin(fwd, speed, volt);
}

//getters

int getIntakeSpeed() {
  return getAxis2Pos() / voltageConversationIntake;
}

int getLIntakeSpeed() { return lIntake.velocity(pct); }
int getRIntakeSpeed() { return rIntake.velocity(pct); }

int getLIntakeEfficiency() { return lIntake.efficiency(); }
int getRIntakeEffiency() { return rIntake.efficiency(); }
int getLIntakePower() { return lIntake.power(); }
int getRIntakePower() { return rIntake.power(); }
double getLIntakeTorque() { return lIntake.torque(); }
double getRIntakeTorque() { return rIntake.torque(); }

int getLIntakeTemp() { return lIntake.temperature(celsius); }
int getRIntakeTemp() { return rIntake.temperature(celsius); }

//doesn't work
bool getLIntakeStress() { 
  if(getLIntakeSpeed() > 80 && getLIntakePower() > 5)
    return true;
  return false; 
}
bool getRIntakeStress() { return rIntake.efficiency(); }

// control

//checks tempeatures of all intake motors are returns in a string which motors are hot

void pauseIntake(){
  intakeState = false;
}

void playIntake(){
  intakeState = true;
}

std::string tempInfoIntake() {
  std::string tempReturn;
  int loopCounter = 0;
  if (getLIntakeTemp() > tempLimitIntake)
    tempReturn = "R ";
    loopCounter++;
  if (getRIntakeTemp() > tempLimitIntake)
    tempReturn += "L ";
    loopCounter++;
  if(loopCounter == 0)
    tempReturn = "All Good";
  return tempReturn;
}