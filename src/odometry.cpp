#include "vex.h"
#include "math.h"
#include "auton-functions.h"

// Global constants
const double trackWidth = 4.625;
const double convertInches = (2.75 * M_PI) / 360;
const bool isDebug = true;

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
    double prevHead = 0;

  while (true) {
    currentLeft = getLeftEncoderRotation() * convertInches; 
    currentRight = -getRightEncoderRotation() * convertInches; 
    deltaL = currentLeft - previousLeft;
    deltaR = currentRight - previousRight;
    head = Inertial.heading(degrees) * (M_PI / 180);
    //printf(" currentLeft: %f", currentLeft);
    ///printf(" currRight: %f", currentRight);
    theta = head - prevHead;

    //theta = ((deltaR - deltaL) / trackWidth);
    printf(" theta: %f", theta);

    if (theta < 0.01 && theta > -0.01) {
      linearDistance = (deltaL + deltaR) / 2;
      if (head < 0.08 || head > 6.2) {
        deltaX = 0;
        deltaY = linearDistance;
      } 
      else {
        deltaX = linearDistance * cos(head);
        deltaY = linearDistance * sin(head);
      }
    } else {
      lRadius = deltaL / theta; 
      linearDistance = 2 * (lRadius + (trackWidth / 2)) * sin(theta / 2); 
      deltaX = linearDistance * cos(head + theta / 2);
      deltaY = linearDistance * sin(head + theta / 2);
    }

    //head = Inertial.heading(degrees) * (M_PI / 180);
    printf(" head: %f", head);
    //printf(" linear dist: %f", linearDistance); */
    /*printf(" head: %f /n", (head * (180 / M_PI)));
    printf(" Inertial value: %f /n", Inertial.heading(degrees));*/
    coor.xPos += deltaX; 
    coor.yPos += deltaY;

    Brain.Screen.print(coor.xPos);
    Brain.Screen.print(coor.yPos);
    printf(" xPos: %f", coor.xPos);
    printf(" yPos: %f \n", coor.yPos); 

    previousLeft = currentLeft;
    previousRight = currentRight;
    prevHead = head;
    
    wait(10, msec); 
    Brain.Screen.clearLine();
  }
  
  return 0;
}

int getPosition()
{
    double currentLeft = 0;
    double currentRight = 0;
    double previousLeft = 0;
    double previousRight = 0;
    double deltaL = 0;
    double deltaR = 0;
    double deltaX = 0;
    double deltaY = 0;
    double linearDistance = 0;
    double headRad = 0;
  	double thetaEncoderRad = 0;
	  double thetaInertialRad = 0;
    double thetaRad = 0;
    double lRadius = 0;
    double prevHeadRad = 0;

    while (true)
    {
      // Reading the odometry encoders
      currentLeft = getLeftEncoderRotation() * convertInches; 
      currentRight = -getRightEncoderRotation() * convertInches; 
      deltaL = currentLeft - previousLeft;
      deltaR = currentRight - previousRight;
      thetaEncoderRad = atan2((deltaR - deltaL), trackWidth);

      // Read the heading from the inertial sensor.
      headRad = Inertial.heading(degrees) * (M_PI / 180);
  
      // Convert head from clockwise angle to counterclockwise (unit circle-based) angle
      headRad = fmod(((2.5 * M_PI) - headRad), (2 * M_PI)); 
      thetaInertialRad = headRad - prevHeadRad;
      thetaRad = thetaInertialRad;
	
	    // Check whether the angle is consistent between the two different sensors (Inertial, Encoder).
	    if (isDebug)
		  {
	        if (fabs(thetaEncoderRad - thetaInertialRad) > 0.01)
		      {
              printf("Warning: Intertial and enocoder readings not consistent!\n");
          }

          printf(" currentLeft: %f", currentLeft);
          printf(" currRight: %f", currentRight);
          printf(" theta (radians): %f", thetaRad);
	    }

      // Calculate the incremental linear distance traveled
      if (fabs(thetaRad) < 0.01)
      {
          linearDistance = (deltaL + deltaR) / 2.0;
      }
      else
      {
          lRadius = deltaL / thetaRad; 
          linearDistance = 2 * (lRadius + (trackWidth / 2)) * sin(thetaRad / 2.0); 
      }

      // Calculate the incremental 2-dimensional coordinates.
      deltaX = linearDistance * cos(headRad + (thetaRad / 2.0));
      deltaY = linearDistance * sin(headRad + (thetaRad / 2.0));

      if (isDebug)
	    {
        printf(" head in radians: %f", headRad);
        printf(" head in degrees: %f /n", (headRad * (180 / M_PI)));
        printf(" linear distance: %f", linearDistance);
        printf(" deltaX: %f", deltaX);
        printf(" deltaY: %f", deltaY);
      }

      coor.xPos += deltaX; 
      coor.yPos += deltaY;

      if (isDebug)
	    {
        printf(" xPos: %f", coor.xPos);
        printf(" yPos: %f \n", coor.yPos);
	    }

      previousLeft = currentLeft;
      previousRight = currentRight;
      prevHeadRad = headRad;
  
      wait(50, msec); 
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
  lFront.setStopping(coast);
  lBack.setStopping(coast);
  rBack.setStopping(coast);
  rFront.setStopping(coast);
  
  Brain.Screen.print("done");
  thread pos(getPosition); // We intend to run this line as a thread so that it can update
                         // xPos & yPos (localized to this skills() function) as the program 
                         // is running. We plan to reference xPos & yPos frequently in other functions
                         // throughout our program (by passing them in as parameters) 
                         // in order to determine the robot's path of motion. */
                      
}
