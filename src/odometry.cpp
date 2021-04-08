#include "vex.h"
#include "autonfunctions.h"

//fields
double currentLeft;
double currentRight;
double previousLeft;
double previousRight;
double deltaL;
double deltaR;
double deltaX;
double deltaY;
double linearDistance;
double head;
double theta;
double lradius;

//constants
const double trackWidth = 4.625;

int getPos(double& xPos, double& yPos) {
  
  while (true) {
    // odometryWheelToInch is a constant global to all our program files... its value is (2.75 * M_PI) / 360
    currentLeft = getLeftEncoderRotation() * odometryWheelToInch; 
    currentRight = -getRightEncoderRotation() * odometryWheelToInch; 
    deltaL = currentLeft - previousLeft;
    deltaR = currentRight - previousRight;
    
    theta = ((deltaR - deltaL) / trackWidth);
    lradius = deltaL / theta;
    linearDistance = 2 * (lradius + (trackWidth / 2)) * sin(theta / 2);

    deltaX = linearDistance * cos(head + theta / 2);
    deltaY = linearDistance * sin(head + theta / 2);

    // Issue #1: When we change the type of these variables (tempX & tempY) to a double or float (as we intend to do), 
    //            xPos and yPos no longer change (they always remain at 0).
    // Issue #2: When tempX & tempY are integers as written below, xPos and yPos change when we move the robot; however, (1) all
    //           precision is lost, and (2) xPos and yPos reset when the robot stops moving. 
    int tempX = xPos;
    int tempY = yPos;

    head += theta;
    xPos = tempX + deltaX; // We tried xPos += deltaX, but that didn't work for some reason; 
                          // thus, we stored its original value in tempX (above) and used that... Same for Y below...
    yPos = tempY + deltaY;
    
    previousLeft = currentLeft;
    previousRight = currentRight;
    
    wait(10, msec); 
  }
    return 0;
}

void skills() {

  double xPos = 0;
  double yPos = 0;

  getPos(xPos, yPos); // We intend to run this line as a thread so that it can update
                      // xPos & yPos (localized to this skills() function) as the program 
                      // is running. We plan to reference xPos & yPos frequently in other functions
                      // throughout our program (by passing them in as parameters) 
                      // in order to determine the robot's path of motion. 
                      
}

/*int setPos(double x, double y) {
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
} */
