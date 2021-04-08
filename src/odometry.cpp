#include "vex.h"
#include "autonfunctions.h"

//variables
double currentLeft;
double currentRight;
double previousLeft;
double previousRight;
double deltaL;
double deltaR;
double deltaX;
double deltaY;
 //double xPos111;
 //double yPos111;
double linearDistance;
double head;
double theta;
double lradius;

thread positionTracker;

//constants
const double trackWidth = 4.625;

//getters
int getPos(double &xPos111, double &yPos111) {
  //while(true) {

    currentLeft = getLeftEncoderRotation() * odometryWheelToInch; 
    currentRight = -getRightEncoderRotation() * odometryWheelToInch; 
    deltaL = currentLeft - previousLeft;
    deltaR = currentRight - previousRight;
    
    theta = ((deltaR - deltaL) / trackWidth);
    lradius = deltaL / theta;
    linearDistance = 2 * (lradius + (trackWidth / 2)) * sin(theta / 2);

    /*Brain.Screen.print("deltaX: ");   
    Brain.Screen.print(deltaX);
            Brain.Screen.print("\r\n ");

    Brain.Screen.print("deltaY ");
    Brain.Screen.print(deltaY); 
            Brain.Screen.print("\r\n "); */

    deltaX = linearDistance * cos(head + theta / 2);
    deltaY = linearDistance * sin(head + theta / 2);

    Brain.Screen.print("Before ");

    /*Brain.Screen.print("deltaX: ");   
    Brain.Screen.print(deltaX);

    Brain.Screen.print("deltaY ");
    Brain.Screen.print(deltaY); */

    /* Brain.Screen.print(head); 
            Brain.Screen.print("\r\n "); */

    /////////////////////////// THIS IS A SOURCE ISSUE -- WHEN WE CHANGE TO DOUBLE, BOTH BEFORE & AFTER PRINT STATEMENTS ARE 0s, SAME 
    ///////// W/ FLOAT; ALSO, THE SECOND ISSUE IS THAT EVERYTHING RESETS WHEN WE STOP MOVING THE ROBOT
    int tempX = xPos111;
    int tempY = yPos111;

    head += theta;
    
   // Brain.Screen.print(xPos111);
   //         Brain.Screen.print("\r\n ");
   // Brain.Screen.print(yPos111);
   //         Brain.Screen.print("\r\n ");
   /* Brain.Screen.print(tempX); 
            Brain.Screen.print("\r\n ");
    Brain.Screen.print(tempY); 
            Brain.Screen.print("\r\n ");

    Brain.Screen.print(deltaX); 
            Brain.Screen.print("\r\n ");
    Brain.Screen.print(deltaY); */

    xPos111 = tempX + deltaX;
    yPos111 = tempY + deltaY;
    wait(500, msec); 
    
  /*  Brain.Screen.print(" After ");
    Brain.Screen.print(xPos111);
        Brain.Screen.print("\r\n ");
    Brain.Screen.print(yPos111);

   //         Brain.Screen.print("\r\n ");
   // Brain.Screen.print(tempX); 
   //         Brain.Screen.print("\r\n ");
   // Brain.Screen.print(tempY); 

    //xPos111 += deltaX;
    //yPos111 += deltaY;
    


  /* Brain.Screen.print(head);
            Brain.Screen.print("\r\n "); */
  /* Brain.Screen.print(xPos111);
            Brain.Screen.print("\r\n ");
    Brain.Screen.print(yPos111);
            Brain.Screen.print("\r\n "); 
    Brain.Screen.print(temp111);
            Brain.Screen.print("\r\n "); */

    //wait(100, msec);
    /*Brain.Screen.print(xPos);
            Brain.Screen.print("\r\n ");
    Brain.Screen.print(yPos);
    wait(500, msec);
    Brain.Screen.clearLine();
    
    Controller.Screen.print(xPos);
    Controller.Screen.print("\n ");
    Controller.Screen.print(yPos); */
    Brain.Screen.clearLine();
    return 0;

  //} 
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


void skills() {
  // 60 second program
  double xPos = 0;
  double yPos = 0;

  //int count = 0;

  Brain.Screen.print("skills ");

  while (true) {

 // Brain.Screen.print("count: ");
  //Brain.Screen.print(++count);
 //         Brain.Screen.print("\r\n ");

 //   Brain.Screen.print(xPos);
 //   Brain.Screen.print(" ");
 //   Brain.Screen.print(yPos);

    getPos(xPos, yPos); 
  }
  
}