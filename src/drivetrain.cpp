#include "vex.h"

//thread for drivetrain to respond to joystick movements
int drivetrainControl() {
  while (true) {
    setDrivetrainSpeed(getLeftSpeedInLinear(), getRightSpeedInLinear());
    wait(10, msec);
  }
}

// setters

//sets speed of drivetrain based on left and right velocity inputs
void setDrivetrainSpeed(int leftSpeed, int rightSpeed) {
  lFront.spin(fwd, leftSpeed, volt);
  lBack.spin(fwd, leftSpeed, volt);
  rBack.spin(fwd, rightSpeed, volt);
  rFront.spin(fwd, rightSpeed, volt);
}

void setDrivetrainCreep() {
  lFront.setStopping(coast);
  lBack.setStopping(coast);
  rBack.setStopping(coast);
  rFront.setStopping(coast);
}
void setDrivetrainLock() {
  lFront.setStopping(brake);
  lBack.setStopping(brake);
  rBack.setStopping(brake);
  rFront.setStopping(brake);
}

// getters

//gets movement speed based on joystick location and
//converts to voltage evenly
int getLeftSpeedInLinear() {
  return (getAxis3Pos() + getAxis4Pos()) / voltageConverstion;
}

int getRightSpeedInLinear() {
  return (getAxis3Pos() - getAxis4Pos()) / voltageConverstion;
}

//gets movement speed based on joystick location and 
//runs through a quadratic to slew
int getLeftSpeedInSlew() {
  int newVal;
  int rawVal = getAxis3Pos() + getAxis4Pos();
  if(rawVal > 0)
  {
    newVal = quadraticA * pow(rawVal, powerRatioB);
  }
  else
  {
    newVal = quadraticA * pow(rawVal, powerRatioB);
  }
  return newVal;
}
int getRightSpeedInSlew() {
  int newVal;
  int rawVal = getAxis3Pos() - getAxis4Pos();
  if(rawVal > 0)
  {
    newVal = quadraticA * pow(rawVal, powerRatioB);
  }
  else
  {
    newVal = quadraticA * pow(rawVal, powerRatioB);
  }
  return newVal;
}

int getLFrontSpeed() {return lFront.velocity(pct);}
int getLBackSpeed() {return lBack.velocity(pct);}
int getRBackSpeed() {return rBack.velocity(pct);}
int getRFrontSpeed() {return rFront.velocity(pct);}

int getInertialRotation() { return Inertial.rotation();}
int getInertialHeading() { return Inertial.heading();}

int getLeftEncoderRotation() { return encoderLeft.rotation(deg); }
int getRightEncoderRotation() { return encoderRight.rotation(deg); }

int getLFrontTemp() {return lFront.temperature(celsius);}
int getLBackTemp() {return lBack.temperature(celsius);}
int getRBackTemp() {return rBack.temperature(celsius);}
int getRFrontTemp() {return rFront.temperature(celsius);}

// control

//reset all Motors
void resetDrivetrain() {
  lFront.resetRotation();
  lBack.resetRotation();
  rBack.resetRotation();
  rFront.resetRotation();
}

void resetEncoders() {
  encoderLeft.resetRotation();
  encoderRight.resetRotation();
}

void calibrateInertial() {
  Inertial.calibrate();
  while(Inertial.isCalibrating())
    wait(100, msec);
  Inertial.resetHeading();
  Inertial.resetRotation();
}

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
