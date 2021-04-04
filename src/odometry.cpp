#include "vex.h"

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

int setPos() {
  return 0;
}