#include "vex.h"

//used to check if being controlled by macro
bool intakeState = true;

//move intakes based on right joystick
int intakeControl() {
  while (true) {
    if(intakeState)
      setIntakeSpeed(getIntakeSpeed());
    wait(10,msec);
  }
}

//setters

// sets speed for both intakes based on one speed parameter
void setIntakeSpeed(int speed) {
  lIntake.spin(fwd, speed/voltageConverstion, volt);
  rIntake.spin(fwd, speed/voltageConverstion, volt);
}

void setIntakeCreep() {
  lIntake.setBrake(coast);
  rIntake.setBrake(coast);
}

void setIntakeLock() {
  lIntake.setBrake(brake);
  rIntake.setBrake(brake);
}

//getters

int getIntakeSpeed() { return getAxis2Pos(); }

int getLIntakeSpeed() { return lIntake.velocity(pct); }
int getRIntakeSpeed() { return rIntake.velocity(pct); }

int getLIntakeTemp() { return lIntake.temperature(celsius); }
int getRIntakeTemp() { return rIntake.temperature(celsius); }

// control

void pauseIntake(){
  intakeState = false;
}

void playIntake(){
  intakeState = true;
}

void resetIntakes() {
  lIntake.resetRotation();
  rIntake.resetRotation();
}

//deploy at the beginning of the match
void deploy() {
  setIntakeSpeed(100);
  wait(200, msec);
  setIntakeSpeed(0);
}

//checks tempeatures of all intake motors are returns in a string which motors are hot
std::string tempInfoIntake() {
  std::string tempReturn;
  int loopCounter = 0;
  if (getLIntakeTemp() > tempLimit)
    tempReturn = "R ";
    loopCounter++;
  if (getRIntakeTemp() > tempLimit)
    tempReturn += "L ";
    loopCounter++;
  if(loopCounter == 0)
    tempReturn = "All Good";
  return tempReturn;
}