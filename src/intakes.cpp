#include "intakes.h"

void intakeControl() {
  while (true) {
    setSpeedIntake(getIntakeSpeed());
    wait(10, msec);
    thread(intakeControl).join();
  }
}

//setters

// sets speed for both intakes based on one speed parameter
void setSpeedIntake(int speed) {
  lIntake.spin(fwd, speed, pct);
  rIntake.spin(fwd, speed, pct);
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