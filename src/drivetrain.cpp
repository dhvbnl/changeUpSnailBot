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

double getInertialRotation() { return Inertial.rotation();}
double getInertialHeading() { return Inertial.heading();}

double getLeftEncoderRotation() { return encoderLeft.rotation(deg); }
double getRightEncoderRotation() { return encoderRight.rotation(deg); }

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
  encoderLeft.setRotation(0, degrees);
  encoderRight.setRotation(0, degrees);
  encoderLeft.setPosition(0, degrees);
  encoderRight.setPosition(0, degrees);
}

void calibrateInertial() {
  Inertial.calibrate();
  while(Inertial.isCalibrating())
    wait(100, msec);
  Inertial.resetHeading();
  Inertial.resetRotation();
  Controller.Screen.print("preauton");
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

void drivetrainTurn(double targetdeg) {
   // proportionality constants
  //double kP = 0.45;
  //double kI = 0.002;
  //double kD = 1.0;
  double kP = 0.43;
  double kI = 0.0004;
  double kD = 0.5;

  // PID loop variables
  double error = 3;
  double integral = 0;
  double derivative = 0;
  double prevError = 0;
  double motorPower = 0;
  bool useright = true;
  while (fabs(targetdeg - getInertialHeading()) > 2) {
    // PID loop to determine motorPower at any given point in time
    double head = getInertialHeading();
    double errorright = targetdeg - head;
    if (targetdeg < head) {
      errorright = 360 - head + targetdeg;
    }
    double errorleft = fabs(targetdeg - head);
    if (targetdeg > head) {
      errorleft = 360 + head - targetdeg;
    }
    if (errorright < errorleft) {
      error = errorright;
      useright = true;
    } else {
      error = errorleft;
      useright = false;
    }
    // pid stuff
    integral = integral + error;
    if (error == 0 or error > targetdeg) {
      integral = 0;
    }
    derivative = error - prevError;
    motorPower = (error * kP + integral * kI + derivative * kD);
    prevError = error;

    wait(15, msec);

    // powering the motors
    if (useright) {
      lFront.spin(fwd, motorPower, pct);
      lBack.spin(fwd, motorPower, pct);
      rBack.spin(fwd, -motorPower, pct);
      rFront.spin(fwd, -motorPower, pct);
    } else {
      lFront.spin(fwd, -motorPower, pct);
      lBack.spin(fwd, -motorPower, pct);
      rBack.spin(fwd, motorPower, pct);
      rFront.spin(fwd, motorPower, pct);
    }
  }
  lBack.stop();
  rBack.stop();
  lFront.stop();
  rFront.stop();
}

void timeDrive(double speed, int timeLength) {
  lFront.spin(fwd, speed, volt);
  rFront.spin(fwd, speed, volt);
  lBack.spin(fwd, speed, volt);
  rBack.spin(fwd, speed, volt);
  wait(timeLength, msec);
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();
}

void arcturn (double left, double right, double turnangle) {
  while (getInertialHeading() < turnangle - 2 || getInertialHeading() > turnangle + 2) {
    lFront.spin(fwd, left, volt);
    lBack.spin(fwd, left, volt);
    rFront.spin(fwd, right, volt);
    rBack.spin(fwd, right, volt);
    wait(10, msec);
  }
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();
}