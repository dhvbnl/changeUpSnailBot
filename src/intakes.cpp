#include "intakes.h"

thread intakeControl() {
  while (true) {
    setSpeedIntake(getIntakeSpeed());
    wait(20, msec);
  }
}

//setters

// sets speed for both intakes based on one speed parameter
void setSpeedIntake(int speed) {
  lIntake.setVelocity(speed, pct);
  rIntake.setVelocity(speed, pct);
}

//getters
int getIntakeSpeed() { return Controller.Axis2.position(); }

int getLIntakeSpeed() { return lIntake.velocity(pct); }
int getRIntakeSpeed() { return rIntake.velocity(pct); }

int getLIntakeTemp() { return lIntake.temperature(celsius); }
int getRIntakeTemp() { return rIntake.temperature(celsius); }

// control

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