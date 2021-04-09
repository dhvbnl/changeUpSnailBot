#include "vex.h"
#include "autonfunctions.h"

//constants
const double trackWidth = 4.625;
const double convertinches = (2.75 * M_PI) / 360;
const double convertdegrees = 360 / (2.75 * M_PI);

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
    double lradius = 0;

  while (true) {
    
    // odometryWheelToInch is a constant global to all our program files... its value is (2.75 * M_PI) / 360
    currentLeft = getLeftEncoderRotation() * odometryWheelToInch; 
    currentRight = -getRightEncoderRotation() * odometryWheelToInch; 
    deltaL = currentLeft - previousLeft;
    deltaR = currentRight - previousRight;
    
    /////////theta = ((deltaR - deltaL) / trackWidth);
    ////////lradius = deltaL / theta; 
    ////////linearDistance = 2 * (lradius + (trackWidth / 2)) * sin(theta / 2); // sin(theta/2) here will be 0 if robot is not turning (theoretically)

    deltaX = linearDistance * cos(head + theta / 2);
    deltaY = linearDistance * sin(head + theta / 2);

    // Issue #1: When we change the type of these variables (tempX & tempY) to a double or float (as we intend to do), 
    //            xPos and yPos no longer change (they always remain at 0).
    // Issue #2: When tempX & tempY are integers as written below, xPos and yPos change when we move the robot; however, (1) all
    //           precision is lost, and (2) xPos and yPos reset when the robot stops moving. 
    int tempX = coor.xPos;
    int tempY = coor.yPos;
    Brain.Screen.print(coor.xPos);
    Brain.Screen.print(" ");
    Brain.Screen.print(coor.yPos);

    head += theta;
    coor.xPos = tempX + deltaX; // We tried xPos += deltaX, but that didn't work for some reason; 
                          // thus, we stored its original value in tempX (above) and used that... Same for Y below...
    coor.yPos = tempY + deltaY;

    previousLeft = currentLeft;
    previousRight = currentRight;
    
    wait(10, msec); 
    Brain.Screen.clearLine();
  }
    return 0;
}

void skills() {
  // 60 second program
  double xPos = 0;
  double yPos = 0;
  lFront.setStopping(brake);
  lBack.setStopping(brake);
  rBack.setStopping(brake);
  rFront.setStopping(brake);

  //Brain.Screen.print("skills ");
  driveprofile(20, true);
  Brain.Screen.print("done");
  /* thread pos(getPos); // We intend to run this line as a thread so that it can update
                      // xPos & yPos (localized to this skills() function) as the program 
                      // is running. We plan to reference xPos & yPos frequently in other functions
                      // throughout our program (by passing them in as parameters) 
                      // in order to determine the robot's path of motion. */
                      
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

struct Acceleration {
  double dist;
  bool speedup;
  bool fwd;
} acc;

int speedr = 0;
int speedl = 0;


int accelerater() {
  //acc.dist *= 360 / (2.75 * M_PI);
  
  acc.dist += (encoderLeft.rotation(degrees) * convertinches);
  double rorig = fabs(encoderRight.rotation(degrees)) * convertinches;
  if (acc.speedup) {
    while ((fabs(encoderRight.rotation(degrees)) * convertinches) < acc.dist) {
      speedr = (4.410714286 * pow((fabs(encoderRight.rotation(degrees)) * convertinches) - rorig, 2)) - (24.32738095 * ((fabs(encoderRight.rotation(degrees)) * convertinches) - rorig)) + 28.875; //quadratic
      speedr = pow(2, ((fabs(encoderRight.rotation(degrees)) * convertinches) - rorig));
      if (!acc.fwd) speedr *= -1;
      if (speedr > 100) speedr = 100;
      speedr /= 8.33;      
      //Brain.Screen.print(fabs(encoderRight.rotation(degrees)));
      //Brain.Screen.print(" ");
      //Brain.Screen.print(acc.dist);
      wait(10, msec);
      //Brain.Screen.clearLine();
    }
    //Brain.Screen.print("finished");
  } else {
    int n = 0;
    if (acc.dist < 8) {
      n = 8 - acc.dist;
    }
    while ((fabs(encoderRight.rotation(degrees)) * convertinches) < acc.dist) {
      //speedr = (4.410714286 * pow((fabs(encoderRight.rotation(degrees)) * convertinches) - rorig, 2)) - (55.06547619 * ((fabs(encoderRight.rotation(degrees)) * convertinches) - rorig)) + 167.1964286; //quadratic
      speedr = 256 * pow(0.5, (((fabs(encoderRight.rotation(degrees))) * convertinches) - rorig) + n); //exponential
      if (!acc.fwd) speedr *= -1;
      if (speedr > 100) speedr = 100;
      speedr /= 8.33;
      wait(10, msec);
    }
  }
  return 0;
}

void driveprofile (int dist, bool fwd) {
  
  Inertial.resetRotation();
  int acceldist = 0;
  
  double getr = 0;
  if (dist > 8) {
    acceldist = 15;
  } else {
    acceldist = dist / 2;
  }
  dist *= convertdegrees;
  double actdist = fabs(encoderRight.rotation(degrees)) + dist;
  if (fwd) {
    acc.dist = acceldist;
    acc.speedup = true;
    acc.fwd = fwd;
  
    acceldist *= convertdegrees;

    thread thread1(accelerater); 

    //accelerate
    int targetL = fabs(encoderLeft.rotation(degrees)) + acceldist;
    int targetR = fabs(encoderRight.rotation(degrees)) + acceldist;
    while ((fabs(encoderLeft.rotation(degrees)) < targetL) && 
      fabs(encoderRight.rotation(degrees)) < targetR) {
/*       Brain.Screen.clearScreen();
      Brain.Screen.print(encoderLeft.rotation(degrees));
      Brain.Screen.print(" ");
      Brain.Screen.print(targetL);
      Brain.Screen.print(" ");
      Brain.Screen.print(fabs(encoderRight.rotation(degrees)));
      Brain.Screen.print(" ");
      Brain.Screen.print(targetR); */
      lFront.spin(vex::directionType::fwd, speedr , volt); 
      lBack.spin(vex::directionType::fwd, speedr , volt);
      rFront.spin(vex::directionType::fwd, speedr, volt);
      rBack.spin(vex::directionType::fwd, speedr, volt); 
      wait(100, msec);
    }
    if (accelerater() == 0) thread1.interrupt();
    getr = fabs(encoderRight.rotation(degrees));
    Brain.Screen.clearScreen();
    Brain.Screen.print("done");
    while (getr < (actdist - (2 * acceldist))) {
      lFront.spin(vex::directionType::fwd, 12 , volt); 
      lBack.spin(vex::directionType::fwd, 12 , volt);
      rFront.spin(vex::directionType::fwd, 12, volt);
      rBack.spin(vex::directionType::fwd, 12, volt);

      /* if (Inertial.rotation(degrees) > 1) {
        while (Inertial.rotation(degrees) > 1) {
          float leftincrease = Inertial.rotation(degrees) / 35 + 1; //probably need to fix this too lazy to do rn
          rFront.spin(vex::directionType::fwd, 12 * leftincrease,
                      volt);
          rBack.spin(vex::directionType::fwd, 12 * leftincrease,
                     volt);
        }
      }
      if (Inertial.rotation(degrees) < -1) {
        while (Inertial.rotation(degrees) < -1) {
          float rightincrease = Inertial.rotation(degrees) / -35 + 1;
          lFront.spin(vex::directionType::fwd, 12 * rightincrease,
                      volt);
          lBack.spin(vex::directionType::fwd, 12 * rightincrease,
                     volt);
        }
      } */
    }

    acc.speedup = false;
    thread thread3(accelerater); 

    //decelerate 
    Brain.Screen.clearScreen();
    Brain.Screen.print("done2");
    thread3.interrupt();
    targetL = fabs(encoderLeft.rotation(degrees)) + acceldist;
    targetR = fabs(encoderRight.rotation(degrees)) + acceldist;
    while ((fabs(encoderLeft.rotation(degrees)) < targetL) && 
      fabs(encoderRight.rotation(degrees)) < targetR) {
      lFront.spin(vex::directionType::fwd, speedr , volt);
      lBack.spin(vex::directionType::fwd, speedr , volt);
      rFront.spin(vex::directionType::fwd, speedr, volt);
      rBack.spin(vex::directionType::fwd, speedr, volt);
    }
    lFront.stop();
    lBack.stop();
    rFront.stop();
    rBack.stop();
  }
}
