#include "drivetrain.h"
#include "controller.h"

const int tempLimit = 55;
const double quadraticA = 0.01;

thread drivetrainControl() {
  while (true) {
    setSpeed(getLeftSpeedIn(), getRightSpeedIn());
    wait(20, msec);
  }
}

// setters
void setSpeed(int leftSpeed, int rightSpeed) {
  lFront.setVelocity(leftSpeed, pct);
  lBack.setVelocity(leftSpeed, pct);
  rBack.setVelocity(rightSpeed, pct);
  rFront.setVelocity(rightSpeed, pct);
}

// getters
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
const char* tempInfo()
{
 if(getLFrontTemp() > tempLimit) return "Left Front HOT";
 else if(getLBackTemp() > tempLimit) return "Left Back HOT";
 else if(getRBackTemp() > tempLimit) return "Right Back HOT";
 else if(getRFrontTemp() > tempLimit) return "Right Front HOT";
 else return "All Good";
}