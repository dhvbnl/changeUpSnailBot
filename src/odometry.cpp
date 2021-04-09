#include "vex.h"
#include "autonfunctions.h"

//constants
const double trackWidth = 4.625;
const double convertInches = (2.75 * M_PI) / 360;

struct Coordinate {
  double xPos;
  double yPos;
} coor;

int getPos() {
    
    double currentLeft = 0;
    double currentRight = 0;
    double previousLeft = 0;
    double previousRight = 0;
    double deltaL = 0;
    double deltaR = 0;
    double deltaX = 0;
    double deltaY = 0;
    double linearDistance = 0;
    double head = 0;
    double theta = 0;
    double lRadius = 0;

  while (true) {
    currentLeft = getLeftEncoderRotation() * convertInches; 
    currentRight = -getRightEncoderRotation() * convertInches; 
    deltaL = currentLeft - previousLeft;
    deltaR = currentRight - previousRight;
    
    // We think the issues start here... :(
    theta = ((deltaR - deltaL) / trackWidth);
    lRadius = deltaL / theta; 
    linearDistance = 2 * (lRadius + (trackWidth / 2)) * sin(theta / 2); // When the robot moves straight, sin(theta/2) here
                                                                        // should theoretically be 0 if the robot is not turning...
                                                                        // meaning linearDistance could be (unwantedly) becoming 0...

    deltaX = linearDistance * cos(head + theta / 2);
    deltaY = linearDistance * sin(head + theta / 2);

    // Issue #1: When we change the type of these variables (tempX & tempY) to a double or float (as we intend to do), 
    //           xPos and yPos no longer change (they always remain at 0)...
    // Issue #2: When tempX & tempY are integers as written below, xPos and yPos change; however, (1) all
    //           precision is lost, and (2) xPos and yPos continue to increase (very rapidly) even when the robot is no longer moving. 
    int tempX = coor.xPos;
    int tempY = coor.yPos;
   
    /*Brain.Screen.print(coor.xPos);
    Brain.Screen.print(" ");
    Brain.Screen.print(coor.yPos); */

    head += theta;
    coor.xPos = tempX + deltaX; // We tried xPos += deltaX, but that didn't work for some reason; 
                                // thus, we stored its original value in tempX (above) and used that... same for Y below...
    coor.yPos = tempY + deltaY;

    previousLeft = currentLeft;
    previousRight = currentRight;
    
    wait(10, msec); 
    Brain.Screen.clearLine();
  }
    return 0;
}

int setPos(double x, double y) {
  double curr_xPos = coor.xPos;
  double curr_yPos = coor.yPos;
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

void skills() {
  // 60 second program
  lFront.setStopping(brake);
  lBack.setStopping(brake);
  rBack.setStopping(brake);
  rFront.setStopping(brake);
  
  Brain.Screen.print("done");
  /* thread pos(getPos); // We intend to run this line as a thread so that it can update
                         // xPos & yPos (localized to this skills() function) as the program 
                         // is running. We plan to reference xPos & yPos frequently in other functions
                         // throughout our program (by passing them in as parameters) 
                         // in order to determine the robot's path of motion. */
                      
}
