#include "vex.h"
#include "math.h"
#include "auton-functions.h"
#include "motion-profile.h"

// Global constants
const double trackWidth = 4.625;
const double convertInches = (2.75 * M_PI) / 360;
const bool isDebug = true;

struct Coordinate {
  double xPos;
  double yPos;
} coor;

int getPosition()
{
    coor.xPos = 0;
    coor.yPos = 0;
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

      thetaEncoderRad = atan2((deltaR - deltaL), trackWidth); // calculated change in heading 

      // Sensor-based heading reading
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
              printf("Warning: Inertial and enocoder readings not consistent!\n");
          }
          //printf(" deltaL: %f", deltaL);
          //printf(" deltaR: %f", deltaR);
          //printf(" theta (radians): %f", thetaRad);
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

      // Calculate the incremental 2-dimensional coordinates x & y
      deltaX = linearDistance * cos(headRad + (thetaRad / 2.0));
      deltaY = linearDistance * sin(headRad + (thetaRad / 2.0));

      if (isDebug)
	    {
        printf(" head in radians: %f", headRad);
        printf(" head in degrees: %f \n", (headRad * (180 / M_PI)));
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
  double xdist = x - curr_xPos;
  double ydist = y - curr_yPos;
  double hyp = sqrt((xdist * xdist) + (ydist * ydist));
  double refAngle = fabs(atan(ydist/xdist)) * 180 / M_PI;
  
  wait(100, msec);

  if (xdist != 0 || ydist != 0) {
    // Using quadrants to calculate absolute angle to turn to
    // Current position (curr_xPos, curr_yPos) is new "center of origin" for quadrant system
    if (x > curr_xPos && y > curr_yPos) // Quadrant 1 angle
    {
      Controller.Screen.print("1");
      turningBasePID(90 - refAngle);
    } 
    else if (x < curr_xPos && y > curr_yPos) // Quadrant 2 angle
    {
      Controller.Screen.print("2");
      turningBasePID(270 + refAngle);
    } 
    else if (x < curr_xPos && y < curr_yPos) // Quadrant 3 angle
    {
      Controller.Screen.print("3");
      turningBasePID(270 - refAngle);
      printf(" refAngle: %f", refAngle);
    } 
    else if (x > curr_xPos && y < curr_yPos)
    { 
      Controller.Screen.print("4");
      turningBasePID(90 + refAngle);
    } 
    else if (xdist == 0) {
      hyp = fabs(ydist);
      if (ydist > 0) {
        turningBasePID(0);
      } else {
        turningBasePID(180);
      }
    } 
    else if (ydist == 0) {
      hyp = fabs(xdist);
      if (xdist > 0) {
        turningBasePID(90);
      } else {
        turningBasePID(270);
      }
    }
  }
  
  
  printf(" refAng: %f", refAngle);
  driveProfile(hyp, true);
  
  return 0;
} 

void skills() {
  // 60 second program
  lFront.setStopping(brake);
  lBack.setStopping(brake);
  rBack.setStopping(brake);
  rFront.setStopping(brake);
  
  thread pos(getPosition); 
}
