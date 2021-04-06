#include "vex.h"
#include "autonfunctions.h"

//constants
const double trackWidth = 4.625;

//variables
double currentLeft;
double currentRight;
double previousLeft;
double previousRight;
double deltaL;
double deltaR;
double deltaX;
double deltaY;
double xPos;
double yPos;
double linearDistance;
double head;
double theta;
double lradius;

//getters
int getPos() {
  while(true) {
    currentLeft = getLeftEncoderRotation() * odometryWheelToInch; 
    currentRight = -getRightEncoderRotation() * odometryWheelToInch; 
    deltaL = currentLeft - previousLeft;
    deltaR = currentRight - previousRight;
    
    theta = (deltaR - deltaL) / trackWidth;
    lradius = deltaL / theta;
    linearDistance = 2 * (lradius + (trackWidth / 2)) * sin(theta / 2);

    deltaX = linearDistance * cos(head + theta / 2);
    deltaY = linearDistance * sin(head + theta / 2);

    head += theta;
    xPos += deltaX;
    yPos += deltaY;

    previousLeft = currentLeft;
    previousRight = currentRight;
    wait(10, msec);
  } 
  return 0;
}

int setPos(double x, double y) {
  double curr_xPos = xPos;
  double curr_yPos = yPos;
  double xdist = fabs(fabs(x) - fabs(curr_xPos));
  double ydist = fabs((y) - fabs(curr_yPos));
  double hyp = sqrt((xdist * xdist) + (ydist * ydist));
  double refAngle = fabs(atan2(ydist, xdist) * 180 / M_PI);

  // Using quadrants to calculate absolute angle to turn to
  if (x > curr_xPos && y > curr_yPos) {
    Controller.Screen.print("1");
    turningBasePID(90 - refAngle);
  } 
  else if (x < curr_xPos && y > curr_yPos) {
    Controller.Screen.print("2");
    turningBasePID(270 + refAngle);
  } 
  else if (x < curr_xPos && y < curr_yPos) {
    Controller.Screen.print("3");
    turningBasePID(270 - refAngle);
  } 
  else if (x > curr_xPos && y < curr_yPos) { 
    Controller.Screen.print("4");
    turningBasePID(90 + refAngle);
  } 
  else {  
    Controller.Screen.print("5");
  }
  inertialDrive(hyp, 50, true);

  return 0;
}