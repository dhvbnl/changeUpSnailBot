#include "vex.h"

// Global constants
const bool isDebug = false;
const double errorMargin = 0.01;

struct Coordinate {
  double xPos;
  double yPos;
  double deltaH;
  double thetaRad;
  double linearDistance; 
  double headingRad;
} coor; 


/*int getPosition()
{
  double currentLeft = 0.0;
  double currentRight = 0.0;
  double previousLeft = 0.0;
  double previousRight = 0.0;
  double deltaL = 0.0;
  double deltaR = 0.0;
  double deltaX = 0.0;
  double deltaY = 0.0;
  //double linearDistance = 0.0;
  coor.linearDistance = 0.0;
  double headRad = 0.0;
  double thetaEncoderRad = 0.0;
  double thetaInertialRad = 0.0;
  //double thetaRad = 0.0;
  double lRadius = 0.0;
  double prevHeadRad = 0.0;

  coor.xPos = 0.0;
  coor.yPos = 0.0;

  while (true)
  {
      // Reading the odometry encoders
      currentLeft = getLeftEncoderRotation() * convertInches; 
      currentRight = getRightEncoderRotation() * convertInches; 
      deltaL = currentLeft - previousLeft;
      deltaR = currentRight - previousRight;

      thetaEncoderRad = atan((deltaR - deltaL)/trackWidth); // calculated change in heading 

      // Sensor-based heading reading
      headRad = getInertialHeading() * (M_PI / 180);

      // Convert head from clockwise angle to counterclockwise (unit circle-based) angle
      headRad = fmod(((2.5 * M_PI) - headRad), (2 * M_PI)); 
    
      thetaInertialRad = headRad - prevHeadRad;
      coor.thetaRad = thetaInertialRad;
  
      // Check whether the angle is consistent between the two different sensors (Inertial, Encoder).
      if (isDebug)
      {
          if (fabs(thetaEncoderRad - thetaInertialRad) > 0.01)
          {
              printf("Warning: Inertial and enocoder readings are not consistent!\n");
          }

         printf(" deltaL: %f", deltaL);
          printf(" deltaR: %f", deltaR);
          printf(" theta (radians): %f", coor.thetaRad);
      }

      // Calculate the incremental linear distance traveled.
      if (fabs(coor.thetaRad) < 0.01)
      {
          coor.linearDistance = (deltaL + deltaR) / 2.0;
      }
      else
      {
          lRadius = deltaL / coor.thetaRad; 
          coor.linearDistance = 2.0 * (lRadius + (trackWidth / 2.0)) * sin(coor.thetaRad / 2.0);
      }

      // Calculate the incremental 2-dimensional coordinates x & y
      deltaX = coor.linearDistance * cos(headRad + (coor.thetaRad / 2.0));
      deltaY = coor.linearDistance * sin(headRad + (coor.thetaRad / 2.0));

      if (isDebug)
      {
         printf(" head: %f radians (%f degrees)\n", headRad, (headRad * (180 / M_PI)));
          printf(" linear distance: %f", coor.linearDistance);
          printf(" deltaX: %f in", deltaX);
          printf(" deltaY: %f in", deltaY);
      }

      coor.xPos += deltaX; 
      coor.yPos += deltaY;

      encoderH.changed(void(horizontalmove)(void));

      if (isDebug)
      {
          printf(" xPos: %f", coor.xPos);
          printf(" yPos: %f \n", coor.yPos);
      }

      previousLeft = currentLeft;
      previousRight = currentRight;
      prevHeadRad = headRad;

      wait(10, msec); 
      Brain.Screen.clearLine();
  }

  return 0;
}
*/
int getPosition()
{
  double currentLeft = 0.0;
  double currentRight = 0.0;
  double currentH = 0.0; 
  double previousLeft = 0.0;
  double previousRight = 0.0;
  double previousH = 0.0;
  double deltaL = 0.0;
  double deltaR = 0.0;
  double deltaX = 0.0;
  double deltaY = 0.0;
  //double linearDistance = 0.0;
  coor.linearDistance = 0.0;
  //double headRad = 0.0;
  double thetaEncoderRad = 0.0;
  double thetaInertialRad = 0.0;
  //double thetaRad = 0.0;
  double lRadius = 0.0;
  double prevHeadRad = 0.0;

  coor.xPos = 0.0;
  coor.yPos = 0.0;
  coor.headingRad = 0.0;
  coor.deltaH = 0.0;

  while (true)
  {
      // Reading the odometry encoders
      currentLeft = getLeftEncoderRotation() * convertInches; 
      currentRight = getRightEncoderRotation() * convertInches; 
      deltaL = currentLeft - previousLeft;
      deltaR = currentRight - previousRight;
      currentH = encoderH.rotation(deg) * convertInches;
      coor.deltaH = currentH - previousH;

      thetaEncoderRad = atan((deltaR - deltaL)/trackWidth); // calculated change in heading 

      // Sensor-based heading reading
      coor.headingRad = getInertialHeading() * (M_PI / 180);

      // Convert head from clockwise angle to counterclockwise (unit circle-based) angle
      coor.headingRad = fmod(((2.5 * M_PI) - coor.headingRad), (2 * M_PI)); 
    
      thetaInertialRad = coor.headingRad - prevHeadRad; // change in heading using inertial
      coor.thetaRad = thetaInertialRad;
  

      // Calculate the incremental linear distance traveled.
      if (fabs(coor.thetaRad) < 0.01)
      {
          coor.linearDistance = (deltaL + deltaR) / 2.0;
      }
      else
      {
          lRadius = deltaL / coor.thetaRad; 
          coor.linearDistance = 2.0 * (lRadius + (trackWidth / 2.0)) * sin(coor.thetaRad / 2.0);
      }

      // Calculate the incremental 2-dimensional coordinates x & y
      deltaX = coor.linearDistance * cos(coor.headingRad + (coor.thetaRad / 2.0));
      deltaY = coor.linearDistance * sin(coor.headingRad + (coor.thetaRad / 2.0));

      coor.xPos += deltaX; 
      coor.yPos += deltaY;

      //encoderH.changed(horizontalmove);
      if (coor.deltaH > 0.01) {
        horizontalmove();
      }

      previousLeft = currentLeft;
      previousRight = currentRight;
      prevHeadRad = coor.headingRad;
      previousH = currentH;

      wait(10, msec); 
      Brain.Screen.clearLine();
  }

  return 0;
}

void horizontalmove() {
  double refAngle;
  int quad = 0;
  double headingDeg = coor.headingRad * (180 / M_PI);

  if (headingDeg < 90) { // Quad 1
    refAngle  = headingDeg;
    quad = 1;
  } 
  else if (headingDeg < 180) { // Quad 4
    refAngle = 180 - headingDeg;
    quad = 2;
  } 
  else if (headingDeg < 270) { // Quad 3
    refAngle = headingDeg - 180;
    quad = 3;
  } 
  else {
    refAngle = 360 - headingDeg; // Quad 2
    quad = 4;
  }

  double deltaX = cos(refAngle * 180 / M_PI) * fabs(coor.deltaH);
  double deltaY = sin(refAngle * 180 / M_PI) * fabs(coor.deltaH); 

  if (coor.deltaH > 0) { // moving right (positive direction)
    if (quad % 2 == 1) { // Quads 1 or 3
      coor.xPos += deltaX;
      coor.yPos -= deltaY;
    } else { // Quads 2 or 4
      coor.xPos += deltaX;
      coor.yPos += deltaY;
    }
  } 
  else { // moving left (negative direction)
    if (quad % 2 == 1) {
      coor.xPos -= deltaX;
      coor.yPos += deltaY;
    } else {
      coor.xPos -= deltaX;
      coor.yPos -= deltaY;
    }
  }
}

void adjustStraightMotionPath() { 
  while (true) {
    if (fabs(coor.thetaRad) > errorMargin) {
      ////
    }
    wait(200, msec);
  }
}

void test() {
  thread track(getPosition);
  thread adjust(adjustStraightMotionPath);

  adjust.interrupt();
}

double getxPos() {
  return coor.xPos;
}

double getyPos() {
  return coor.yPos;
}

int setPos (double x, double y, bool repeat) {
  
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
      drivetrainTurn(90 - refAngle);
    } 
    else if (x < curr_xPos && y > curr_yPos) // Quadrant 2 angle
    {
      drivetrainTurn(270 + refAngle);
    } 
    else if (x < curr_xPos && y < curr_yPos) // Quadrant 3 angle
    {
      drivetrainTurn(270 - refAngle);
    } 
    else if (x > curr_xPos && y < curr_yPos)
    { 
      drivetrainTurn(90 + refAngle);
    } 
    else if (xdist == 0) 
    {
      Controller.Screen.print(ydist);
      hyp = fabs(ydist);
      if (ydist > 0 && fabs(getInertialHeading() - 0) > 1) {
        drivetrainTurn(0);
      } else if (ydist < 0 && fabs(getInertialHeading() - 0) > 1) {
        drivetrainTurn(180);
      }
    } 
    else if (ydist == 0) 
    {
      hyp = fabs(xdist);
      if (xdist > 0 && fabs(getInertialHeading() - 90) > 1) {
        drivetrainTurn(90);
      } else if (xdist < 0 && fabs(getInertialHeading() - 0) > 1) {
        drivetrainTurn(270);
      }
    }
    wait(100, msec);
    Controller.Screen.clearLine();
    Controller.Screen.print("here");
    printf(" refAng: %f", refAngle);
    driveProfile(hyp, true);
    
    if (repeat) 
    {
      if (fabs(x - coor.xPos) > 1 || fabs(y - coor.yPos) > 1) {
        setPos(x, y, false);
      }
    }
  }
  return 0;
} 

void printPos() {
  printf("x: %f", coor.xPos);
  printf("y: %f\n", coor.yPos);
}
