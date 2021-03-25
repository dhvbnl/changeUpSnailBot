#include "drivetrain.h"
#include "controller.h"

//thread for drivetrain to respond to joystick movements
thread drivetrainControl() {
  while (true) {
    setSpeedDrivetrain(getLeftSpeedIn(), getRightSpeedIn());
    wait(20, msec);
  }
}

// setters

//sets speed of drivetrain based on left and right velocity inputs
void setSpeedDrivetrain(int leftSpeed, int rightSpeed) {
  lFront.setVelocity(leftSpeed, pct);
  lBack.setVelocity(leftSpeed, pct);
  rBack.setVelocity(rightSpeed, pct);
  rFront.setVelocity(rightSpeed, pct);
}

// getters

//gets movement speed based on joystick location and 
//runs through a quadratic to motion profile
int getLeftSpeedIn() {
  int rawVal = getAxis3Pos() + getAxis4Pos();
  int newVal = quadraticA * pow(rawVal, 2);
  return newVal;
}
int getRightSpeedIn() {
  int rawVal = getAxis3Pos() - getAxis4Pos();
  int newVal = quadraticA * pow(rawVal, 2);
  return newVal;
}

int getLFrontSpeed() {return lFront.velocity(pct);}
int getLBackSpeed() {return lBack.velocity(pct);}
int getRBackSpeed() {return rBack.velocity(pct);}
int getRFrontSpeed() {return rFront.velocity(pct);}

int getLFrontTemp() {return lFront.temperature(celsius);}
int getLBackTemp() {return lBack.temperature(celsius);}
int getRBackTemp() {return rBack.temperature(celsius);}
int getRFrontTemp() {return rFront.temperature(celsius);}

// control

//checks tempeatures of all drive motors are returns in a string which motors are hot
std::string tempInfoDrive() {
  std::string tempReturn;
  int loopCounter = 0;
  if (getLFrontTemp() > tempLimit)
    tempReturn = "LF ";
    loopCounter++;
  if (getLBackTemp() > tempLimit)
    tempReturn += "LR ";
    loopCounter++;
  if (getRBackTemp() > tempLimit)
    tempReturn += "RB ";
    loopCounter++;
  if (getRFrontTemp() > tempLimit)
    tempReturn += "RF ";
    loopCounter++;
  if(loopCounter == 0)
    tempReturn = "All Good";
  return tempReturn;
}