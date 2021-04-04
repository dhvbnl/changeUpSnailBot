#include "intakes.h"
#include "controller.h"

bool intakeState = true;

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
  lIntake.spin(fwd, speed, volt);
  rIntake.spin(fwd, speed, volt);
}

void setIntakeCreep() {
  rIntake.setBrake(coast);
  rIntake.setBrake(coast);
}

void setIntakeLock() {
  rIntake.setBrake(brake);
  rIntake.setBrake(brake);
}

//getters

int getIntakeSpeed() {
  return getAxis2Pos() / voltageConversationIntake;
}

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

//checks tempeatures of all intake motors are returns in a string which motors are hot
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