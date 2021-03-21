#include "drivetrain.h"
#include "controller.h"

thread drivetrainControl()
{
  while(true)
  {
    setSpeed(getLeftSpeed(), getRightSpeed());
    wait(20, msec);
  }
}

//setters
void setSpeed(int leftSpeed, int rightSpeed)
{
  lFront.setVelocity(leftSpeed,pct);
  lBack.setVelocity(leftSpeed,pct);
  rBack.setVelocity(rightSpeed,pct);
  rFront.setVelocity(rightSpeed,pct);
}

//getters
int getLeftSpeed()
{
  int rawVal = getAxis3Pos() + getAxis4Pos();
  int newVal = 0.01 * pow(rawVal, 2);
  return newVal;
}

int getRightSpeed()
{
  int rawVal = getAxis3Pos() - getAxis4Pos();
  int newVal = 0.01 * pow(rawVal, 2);
  return newVal;
}