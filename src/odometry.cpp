#include "vex.h"
#include "autonfunctions.h"

//constants
const double trackWidth = 4.625;

struct Coordinate {
  double xPos;
  double yPos;
} coor;

int getPos() {
  
  while (true) {
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


int acceleratel() {
  //bool fwd = true;
  acc.dist *= 360 / (2.75 * M_PI);
  acc.dist += encoderLeft.rotation(degrees);
  double lorig = encoderLeft.rotation(degrees);
  int speedl = 0;
  //int speed = 0;
  if (acc.speedup) {    
    while (encoderLeft.rotation(degrees) < acc.dist) {
      speedl = (17.64285714 * pow(encoderLeft.rotation(degrees) - lorig, 2)) - (48.6547619 * (encoderLeft.rotation(degrees) - lorig)) + 28.875;
      if (!acc.fwd) speedl *= -1;
      if (speedl > 100) speedl = 100;
      speedl /= 8.33;
      Brain.Screen.print("left ");
      Brain.Screen.print(speedl);
      wait(10, msec);
      Brain.Screen.clearLine();
      
      //return speedl;
    }
    
  } else {
    while (encoderLeft.rotation(degrees) < acc.dist) {
      speedl = (17.64285714 * pow(encoderLeft.rotation(degrees) - lorig, 2)) - (110.1309524 * (encoderLeft.rotation(degrees) - lorig)) + 167.1964286;   
      if (!acc.fwd) speedl *= -1;
      if (speedl > 100) speedl = 100;
      speedl /= 8.33;
     // return speedl;
    }
  }
  return 0;
}


int accelerater() {
  /* Controller.Screen.clearScreen();
  Controller.Screen.print(acc.dist); */
  acc.dist *= 360 / (2.75 * M_PI);
  acc.dist += encoderLeft.rotation(degrees);

  double rorig = encoderRight.rotation(degrees);
  Controller.Screen.clearScreen();
  Controller.Screen.print(rorig);
  //int speedr = 0;
  //int count = 0;
  //int speed = 0;
  if (acc.speedup) {
    while (fabs(encoderRight.rotation(degrees)) < acc.dist) {
      speedr = (17.64285714 * pow(fabs(encoderRight.rotation(degrees)) - rorig, 2)) - (48.6547619 * (fabs(encoderRight.rotation(degrees)) - rorig)) + 28.875;
      if (!acc.fwd) speedr *= -1;
      if (speedr > 100) speedr = 100;
      speedr /= 8.33;
      //count++;
     // Brain.Screen.print(count);
      //Brain.Screen.print(" ");
      
      Brain.Screen.print(fabs(encoderRight.rotation(degrees)));
      Brain.Screen.print(" ");
      Brain.Screen.print(acc.dist);
      wait(10, msec);
      Brain.Screen.clearLine();
      //return speedr;
     
    }
    Brain.Screen.print("finished");
  } else {
    while (fabs(encoderRight.rotation(degrees)) < acc.dist) {
      speedr = (17.64285714 * pow(fabs(encoderRight.rotation(degrees)) - rorig, 2)) - (110.1309524 * (fabs(encoderRight.rotation(degrees)) - rorig)) + 167.1964286;
      if (!acc.fwd) speedr *= -1;
      if (speedr > 100) speedr = 100;
      speedr /= 8.33;
      return speedr;
    }
  }
  Brain.Screen.print("hi");
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
  dist *= 360 / (2.75 * M_PI);
  double actdist = fabs(encoderRight.rotation(degrees)) + dist;
  if (fwd) {
    acc.dist = acceldist;
    acc.speedup = true;
    acc.fwd = fwd;
  
  acceldist *= 360 / (2.75 * M_PI);
 
//   thread getspeedl(acceleratel, &acc);
  
  //thread thread2(accelerater); 
  thread thread1(accelerater); 
/*   thread * aaaa = new thread(foo);
  
//thread( int  (* callback)(void *), void *arg );

Acceleration* accc = new Acceleration();


thread abcd(foo1, &acc);
thread* abcd1 = new thread(foo1, &acc); */

    //accelerate
    int targetL = fabs(encoderLeft.rotation(degrees)) + acceldist;
    int targetR = fabs(encoderRight.rotation(degrees)) + acceldist;
    while ((fabs(encoderLeft.rotation(degrees)) < targetL) && 
      fabs(encoderRight.rotation(degrees)) < targetR) {
      Brain.Screen.clearScreen();
      Brain.Screen.print(encoderLeft.rotation(degrees));
      Brain.Screen.print(" ");
      Brain.Screen.print(targetL);
      Brain.Screen.print(" ");
      Brain.Screen.print(fabs(encoderRight.rotation(degrees)));
      Brain.Screen.print(" ");
      Brain.Screen.print(targetR);
  /*     lFront.spin(vex::directionType::fwd, speedr , volt); //is it 12? i think its like speed in pct / 8.33 which is 12 if speed is 100 but im not sure
      lBack.spin(vex::directionType::fwd, speedr , volt);
      rFront.spin(vex::directionType::fwd, speedr, volt);
      rBack.spin(vex::directionType::fwd, speedr, volt);  */
      wait(100, msec);
    }
    if (accelerater() == 0) thread1.interrupt();
    //thread2.interrupt();
    //getl = encoderLeft.rotation(degrees);
    getr = fabs(encoderRight.rotation(degrees));
    Brain.Screen.clearScreen();
    Brain.Screen.print("done");
    while (getr < (actdist - 2 * acceldist)) {
      lFront.spin(vex::directionType::fwd, 12 , volt); //is it 12? i think its like speed in pct / 8.33 which is 12 if speed is 100 but im not sure
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
    //thread thread4(accelerater); 
    //decelerate 
    Brain.Screen.clearScreen();
    Brain.Screen.print("done2");
    thread3.interrupt();
    targetL = fabs(encoderLeft.rotation(degrees)) + acceldist;
    targetR = fabs(encoderRight.rotation(degrees)) + acceldist;
    while ((fabs(encoderLeft.rotation(degrees)) < targetL) && 
      fabs(encoderRight.rotation(degrees)) < targetR) {
      lFront.spin(vex::directionType::fwd, speedr , volt); //is it 12? i think its like speed in pct / 8.33 which is 12 if speed is 100 but im not sure
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
