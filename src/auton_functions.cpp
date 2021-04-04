#include "vex.h"
#include <math.h>
#include <vex_vision.h>
#include "odom.h"


#define M_PI acos(-1.0)
const int lineMax = 68;
const int tempLimitRollers = 55;
int aleftdrivespeed;
int arightdrivespeed;
int aintakespeed;
int arollertopspeed;
int arollerbottomspeed;
int arollertopcontrol;
int arollerbottomcontrol;
double adrivespeed = 1.5;
bool adrivestate = true;
bool aintakestate = true;
bool autonGoal = false;
bool custom = false;
vex::task taskHOLDa;
vex::task taskSHOOTa; // autonShoot ((for In 1)
vex::task taskSHOOTb; // autonShootIn2
vex::task taskINTAKES1;
vex::task taskINTAKES2;
vex::task taskHOLDb;
vex::task taskdeploy;
timer t = timer();    // Timer for moving'
timer loop = timer(); // timer for while loops


void turningBasePID(double targetdeg) {
  lFront.resetPosition();
  lBack.resetPosition();
  rFront.resetPosition();
  rBack.resetPosition();
  // proportionality constants
  float kP = 0.39;
  float kI = 0.0009;
  float kD = 0.5;
  // PID loop variables
  float error = 3;
  float integral = 0;
  float derivative = 0;
  float prevError = 0;
  float motorPower = 0;
  bool useright = true;

  while (error >= 3) {
    // PID loop to determine motorPower at any given point in time
    float head = Inertial.heading(degrees);
    int errorright = targetdeg - head;
    if (targetdeg < head) {
      errorright = 360 - head + targetdeg;
    }
    int errorleft = fabs(targetdeg - head);
    if (targetdeg > head) {
      errorleft = 360 + head - targetdeg;
    }
    if (errorright < errorleft) {
      error = errorright;
      useright = true;
    } else {
      error = errorleft;
      useright = false;
    }
    // pid stuff
    integral = integral + error;
    if (error == 0 or error > targetdeg) {
      integral = 0;
    }
    derivative = error - prevError;
    motorPower = (error * kP + integral * kI + derivative * kD);
    prevError = error;

    wait(15, msec);

    // powering the motors
    if (useright == true) {
      lBack.spin(vex::directionType::fwd, motorPower, vex::velocityUnits::pct);
      lFront.spin(vex::directionType::fwd, motorPower, vex::velocityUnits::pct);
      rBack.spin(vex::directionType::fwd, -motorPower, vex::velocityUnits::pct);
      rFront.spin(vex::directionType::fwd, -motorPower,
                  vex::velocityUnits::pct);
    }

    else if (useright == false) {
      lBack.spin(vex::directionType::fwd, -motorPower, vex::velocityUnits::pct);
      lFront.spin(vex::directionType::fwd, -motorPower,
                  vex::velocityUnits::pct);
      rBack.spin(vex::directionType::fwd, motorPower, vex::velocityUnits::pct);
      rFront.spin(vex::directionType::fwd, motorPower, vex::velocityUnits::pct);
    }
  }
  lBack.stop();
  rBack.stop();
  lFront.stop();
  rFront.stop();
}

void curve(int deg, int left, int right) {
  while (Inertial.heading(degrees) < deg - 2 ||
         Inertial.heading(degrees) > deg + 2) {
    lFront.spin(vex::directionType::fwd, left, vex::velocityUnits::rpm);
    lBack.spin(vex::directionType::fwd, left, vex::velocityUnits::rpm);
    rFront.spin(vex::directionType::fwd, right, vex::velocityUnits::rpm);
    rBack.spin(vex::directionType::fwd, right, vex::velocityUnits::rpm);
  }
  Brain.Screen.print(Inertial.heading(degrees));
  lFront.stop();
  lBack.stop();
  rFront.stop();
  rBack.stop();
}
void detectBall() {
  int CoorX = 170;
  int CoorY = 180;
  Vision.takeSnapshot(Vision__REDBALL);
  Controller1.Screen.print("snap");
  wait(2000, msec);
  float theta = ((atan2((Vision.largestObject.centerX - CoorX),
                        Vision.largestObject.centerY) *
                  180) /
                 M_PI);
  int abspos = (int)(theta + Inertial.heading(degrees));
  Controller1.Screen.clearLine();
  Controller1.Screen.print(theta);
  if (Vision.largestObject.centerX > CoorX + 5) { // turning right
    if (abspos > 359) {
      abspos -= 360;
    }
    turningBasePID(abspos);
    /*lFront.spin(vex::directionType::fwd, 10, vex::velocityUnits::rpm);
    lBack.spin(vex::directionType::fwd, 10, vex::velocityUnits::rpm);
    rFront.spin(vex::directionType::rev, 10, vex::velocityUnits::rpm);
    rBack.spin(vex::directionType::rev, 10, vex::velocityUnits::rpm); */
  } else if (Vision.largestObject.centerX < CoorX - 5) {
    if (abspos < 0) {
      abspos += 360;
    }
    turningBasePID(abspos);
    /*lFront.spin(vex::directionType::rev, 10, vex::velocityUnits::rpm);
    lBack.spin(vex::directionType::rev, 10, vex::velocityUnits::rpm);
    rFront.spin(vex::directionType::fwd, 10, vex::velocityUnits::rpm);
    rBack.spin(vex::directionType::fwd, 10, vex::velocityUnits::rpm); */
  } else {
    lFront.stop();
    lBack.stop();
    rFront.stop();
    rBack.stop();
    Brain.Screen.print("stop");
    Brain.Screen.print(Vision.largestObject.centerX);
    Brain.Screen.clearLine();
  }
  Brain.Screen.clearLine();
  Brain.Screen.print("done");

  Brain.Screen.newLine();
}
void driveInertial(double dist, int speed, bool fwd) {
 // speed /= 8.33;
  // dist = dist * (360 / 20.25);
  Inertial.resetRotation();
  rightEncoder.resetRotation();
  leftEncoder.resetRotation();
  double inchessf = 360 / (2.75 * M_PI);
  dist *= inchessf;
  while (fabs(rightEncoder.rotation(degrees)) < dist &&
         fabs(leftEncoder.rotation(degrees)) < dist) {
    if (fwd) {
      // Controller1.Screen.print(dist);
      lFront.spin(vex::directionType::fwd, speed, pct);
      lBack.spin(vex::directionType::fwd, speed, pct);
      rFront.spin(vex::directionType::fwd, speed, pct);
      rBack.spin(vex::directionType::fwd, speed, pct);
      if (Inertial.rotation(degrees) > 1) {
        while (Inertial.rotation(degrees) > 1) {
          float leftincrease = Inertial.rotation(degrees) / 35 + 1;
          rFront.spin(vex::directionType::fwd, speed * leftincrease,
                      pct);
          rBack.spin(vex::directionType::fwd, speed * leftincrease,
                     pct);
        }
      }
      if (Inertial.rotation(degrees) < -1) {
        while (Inertial.rotation(degrees) < -1) {
          float rightincrease = Inertial.rotation(degrees) / -35 + 1;
          lFront.spin(vex::directionType::fwd, speed * rightincrease,
                      pct);
          lBack.spin(vex::directionType::fwd, speed * rightincrease,
                     pct);
        }
      }
    } else {
      // Controller1.Screen.print(dist);
      lFront.spin(vex::directionType::fwd, -speed, pct);
      lBack.spin(vex::directionType::fwd, -speed, pct);
      rFront.spin(vex::directionType::fwd, -speed, pct);
      rBack.spin(vex::directionType::fwd, -speed, pct);
      if (Inertial.rotation(degrees) > 1) {
        while (Inertial.rotation(degrees) > 1) {
          float leftincrease = Inertial.rotation(degrees) / 35 + 1;
          lFront.spin(vex::directionType::fwd, -speed * leftincrease,
                      pct);
          lBack.spin(vex::directionType::fwd, -speed * leftincrease,
                     pct);
        }
      }
      if (Inertial.rotation(degrees) < -1) {
        while (Inertial.rotation(degrees) < -1) {
          float rightincrease = Inertial.rotation(degrees) / -35 + 1;
          rFront.spin(vex::directionType::fwd, -speed * rightincrease,
                      pct);
          rBack.spin(vex::directionType::fwd, -speed * rightincrease,
                    pct);
        }
      }
    }
  }
  // Controller1.Screen.print("stop");
  lBack.stop();
  rBack.stop();
  lFront.stop();
  rFront.stop();
}

int timeDrive(int ms, int speed, bool fwd) {
  rightEncoder.resetRotation();
  leftEncoder.resetRotation();
  Inertial.resetRotation();
  // dist = dist * (360 / 20.25);
  t.clear();
  while (t.time() < ms) {
    if (fwd) {
      lFront.spin(vex::directionType::fwd, speed, vex::velocityUnits::rpm);
      lBack.spin(vex::directionType::fwd, speed, vex::velocityUnits::rpm);
      rFront.spin(vex::directionType::fwd, speed, vex::velocityUnits::rpm);
      rBack.spin(vex::directionType::fwd, speed, vex::velocityUnits::rpm);
      if (Inertial.rotation(degrees) > 1) {
        while (Inertial.rotation(degrees) > 1) {
          if (t.time() >= ms) {
            return 0;
          }
          float leftincrease = Inertial.rotation(degrees) / 35 + 1;
          rFront.spin(vex::directionType::fwd, speed * leftincrease,
                      vex::velocityUnits::rpm);
          rBack.spin(vex::directionType::fwd, speed * leftincrease,
                     vex::velocityUnits::rpm);
        }
      }
      if (Inertial.rotation(degrees) < -1) {
        while (Inertial.rotation(degrees) < -1) {
          if (t.time() >= ms) {
            return 0;
          }
          float rightincrease = Inertial.rotation(degrees) / -35 + 1;
          lFront.spin(vex::directionType::fwd, speed * rightincrease,
                      vex::velocityUnits::rpm);
          lBack.spin(vex::directionType::fwd, speed * rightincrease,
                     vex::velocityUnits::rpm);
        }
      }
    } else {
      lFront.spin(vex::directionType::fwd, -speed, vex::velocityUnits::rpm);
      lBack.spin(vex::directionType::fwd, -speed, vex::velocityUnits::rpm);
      rFront.spin(vex::directionType::fwd, -speed, vex::velocityUnits::rpm);
      rBack.spin(vex::directionType::fwd, -speed, vex::velocityUnits::rpm);
      if (Inertial.rotation(degrees) > 1) {
        while (Inertial.rotation(degrees) > 1) {
          if (t.time() >= ms) {
            return 0;
          }
          float leftincrease = Inertial.rotation(degrees) / 35 + 1;
          lFront.spin(vex::directionType::fwd, -speed * leftincrease,
                      vex::velocityUnits::rpm);
          lBack.spin(vex::directionType::fwd, -speed * leftincrease,
                     vex::velocityUnits::rpm);
        }
      }
      if (Inertial.rotation(degrees) < -1) {
        while (Inertial.rotation(degrees) < -1) {
          if (t.time() >= ms) {
            return 0;
          }
          float rightincrease = Inertial.rotation(degrees) / -35 + 1;
          rFront.spin(vex::directionType::fwd, -speed * rightincrease,
                      vex::velocityUnits::rpm);
          rBack.spin(vex::directionType::fwd, -speed * rightincrease,
                     vex::velocityUnits::rpm);
        }
      }
    }
  }
  // Controller1.Screen.print("stop");
  lBack.stop();
  rBack.stop();
  lFront.stop();
  rFront.stop();
  return 0;
}

void stopDrive() {
  lBack.stop();
  rBack.stop();
  lFront.stop();
  rFront.stop();
}


void autonmain(void) {

  //Controller1.Screen.print(rollerSpeed);
  //shootAllRollers();
  lFront.setStopping(vex::brakeType::brake);
  rFront.setStopping(vex::brakeType::brake);
  lBack.setStopping(vex::brakeType::brake);
  rBack.setStopping(vex::brakeType::brake);
  // STEP 1 ****************************
  // Moving fwd to intake ball
  lIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  // rollerBottom.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  wait(500, msec);
  lIntake.stop();
  rIntake.stop(); 
  //driveInertial(30, 50, true); 
  // rollerBottom.stop();
  wait(10, msec);
  // Controller1.Screen.clearLine();
  //taskHOLDb = vex::task(autonHoardone);
  hoardAll = thread(hoardAllRollers);
  driveInertial(23, 40, true);

  //loop.clear();
  //while (autonHoardone() != 0 && loop.time() < 1000) {
  //}

  // Turn 90 deg to intake 2nd ball
  turningBasePID(89);
  wait(200, msec); 
  //taskHOLDa = vex::task(autonHoard);
  //loop.clear(); */
  timeDrive(1800, 50, true);
  // Move back & turn twd corner goal; shoot balls (2 blue balls are being
  // intaked)
  driveInertial(25, 40, false);
  hoardAll.interrupt();
  turningBasePID(132);
  //loop.clear();
  timeDrive(2000, 50, true);
  shootAllRollers();
  wait(50, msec);
  descoreTwo = thread(descoreTwoBalls);
  wait(100, msec);
  descoreTwo.interrupt();
  //taskSHOOTa = vex::task(autonShoot);
  // loop.clear();
  //while (autonShoot() != 0) {
  //}
  driveInertial(15, 50, false);  



  // outtakke the 2 blue balls
  //taskCLEANa = vex::task(autonClean);
  //while (autonClean() != 0) {
  //}
 
  // STEP 2 ******************************
   /* turningBasePID(217);
//   turningBasePID(330);
 // taskHOLDb = vex::task(autonHoardone);
  driveInertial(63, 70, true);
 // driveInertial(64, 70, true);
  turningBasePID(90); */ /*
  if (lineTop.value(percentUnits::pct) >= 70) {
    taskHOLDb = vex::task(autonHoardone);
    // timeDrive(2430, 50, true);
    timeDrive(2400, 50, true);
    while (autonHoardone() != 0) {
    }
  } else {
    taskHOLDa = vex::task(autonHoard);
    // timeDrive(2430, 50, true);
    timeDrive(2400, 50, true);
    while (autonHoard() != 0) {
    }
  }
  Brain.Screen.clearScreen();
  Brain.Screen.print(autonGoal == false);
  autonGoal = true;
  custom = false;
  taskSHOOTa = vex::task(autonShoot);
  loop.clear();
  while (loop.time() < 1500) {
  } */

  // Step 3 ****************************
/*   driveInertial(15, 70, false);
  // taskCLEANa = vex::task(autonClean);
  while (autonClean() != 0) {
  }
  // turningBasePID(175, true);
  turningBasePID(270);
  lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::pct);
  int val = timeDrive(1500, 60, true);
  for (int i = 0; i < 5; i++) {
    timeDrive(700, 40, true);
    timeDrive(700, 20, false);
  }
  // wait(2300, msec);
  // while (val != 0) {}
  lIntake.stop();
  rIntake.stop();
  lFront.stop();
  lBack.stop();
  rFront.stop();
  rBack.stop();
  custom = false; */
  // taskSHOOTa = vex::task(autonShoot);
  // taskCLEANa = vex::task(autonClean);

  /*
    //Step 4
    timeDrive(400, 50, false);
    driveInertial(10, 50, false);
    turningBasePID(12);
    //driveInertial(33, 70, true);
    taskHOLDb = vex::task(autonHoardone);
    loop.clear();
          driveInertial(18, 70, true);
          driveInertial(5, 50, false);
          turningBasePID(90);
    Controller1.Screen.clearLine();
    while (autonHoardone() != 0 && loop.time() < 2000) {
      Controller1.Screen.print("hi");
      lFront.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
      rFront.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
      lBack.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
      rBack.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
    }
    driveInertial(5, 50, false);
    if (lineTop.value(percentUnits::pct) >= 70) {
      taskHOLDb = vex::task(autonHoardone);
      //timeDrive(2430, 50, true);
      timeDrive(1600, 50, true);
      while (autonHoardone() != 0) {}
    } else {
      taskHOLDa = vex::task(autonHoard);
      //timeDrive(2430, 50, true);
      timeDrive(1600, 50, true);
      while (autonHoard() != 0) {}
    }
    driveInertial(27, 60, false);
    turningBasePID(40);
    loop.clear();

    timeDrive(2000, 50, true);

    autonGoal = false;

    taskSHOOTa = vex::task(autonShoot);
    //loop.clear();
    while (autonShoot() != 0) {}
    driveInertial(15, 50, false);

    // outtakke the 2 blue balls
    taskCLEANa = vex::task(autonClean);
    while (autonClean() != 0) {} */
}

void testVision(void) {
  while (1 == 1) {
    // if (Vision5.objectCount > 0) {
    Controller1.Screen.print("Red Ball Found");
    //}
  }
}

/* void teamworkAuton() {
  lFront.setStopping(vex::brakeType::coast);
  rFront.setStopping(vex::brakeType::coast);
  lBack.setStopping(vex::brakeType::coast);
  rBack.setStopping(vex::brakeType::coast);
  }

  // detectBall();
  // STEP 1 ****************************
  // Moving fwd to intake ball
  lIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  // rollerBottom.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  wait(500, msec);
  lIntake.stop();
  rIntake.stop();
  // rollerBottom.stop();
  wait(10, msec);
  taskHOLDa = vex::task(autonHoard);
  //loop.clear();
  curve(85, 100, 69);
  Controller1.Screen.print(loop.time());
  lFront.stop();
  rFront.stop();
  rBack.stop();
  lBack.stop();
  wait(300, msec);

  autonShoot1(1);
  wait(50, msec);
  //STEP 2
  //timeDrive(300, 70, false);
  //curve(45, -100, -88);
  //driveInertial(90, 80, false);
  timeDrive(300, 60, false);
  turningBasePID(80);
  timeDrive(2400, 100, false);
  turningBasePID(135);
  timeDrive(1564, 100, true);
  autonShoot1(2);
   //turningBasePID(45);
  //driveInertial(20, 80, false);

  //driveInertial(3, 90, false);
  //wait(100, msec);
   //turningBasePID(135);
  //driveInertial(24, 68, true);
  //timeDrive(1400, 68, true);
  //autonShoot1(2);
  //wait(130, msec);
  //driveInertial(47, 80, false);

  //STEP 3
  //turningBasePID(189);
  timeDrive(1520, 100, false);
  //driveInertial(40, 70, false);
  turningBasePID(187);

  taskHOLDa = vex::task(autonHoardone);
  timeDrive(2400, 100, true);
  wait(90, msec);
  autonShoot1(2);
  wait(50, msec);
  driveInertial(5, 90, false);

} */
int deploy() {
  lIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  // rollerBottom.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  wait(500, msec);
  lIntake.stop();
  rIntake.stop();
  return 0;
}


void teamworkAuton() {
/*   lFront.setStopping(vex::brakeType::coast);
  rFront.setStopping(vex::brakeType::coast);
  lBack.setStopping(vex::brakeType::coast);
  rBack.setStopping(vex::brakeType::coast);
  Inertial.resetHeading(); */
  /*while (true){
    Controller1.Screen.clearLine();
    Controller1.Screen.print(Inertial.heading(degrees));
    wait(200, msec);

  }*/

  // detectBall();
  // STEP 1 ****************************
  // Moving fwd to intake ball
  /* lIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  // rollerBottom.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  wait(500, msec);
  lIntake.stop();
  rIntake.stop();
  // rollerBottom.stop();
  wait(10, msec); */
/*   taskHOLDa = vex::task(autonHoard);
  // loop.clear();
  curve(85, 100, 69);
  Controller1.Screen.print(loop.time());
  lFront.stop();
  rFront.stop();
  rBack.stop();
  lBack.stop();
  wait(300, msec);

  autonShoot1(1);
  wait(50, msec); */
  // STEP 2
  /*   timeDrive(300, 70, false);
    curve(45, -100, -88); */
  // driveInertial(90, 80, false);
/*   timeDrive(900, 80, false);
  turningBasePID(45);
  timeDrive(1700, 90, false);
  turningBasePID(135);
  timeDrive(600, 70, true);
  autonShoot1(2); */
  /* turningBasePID(45);
  driveInertial(20, 80, false);  */

  // driveInertial(3, 90, false);
  // wait(100, msec);
  /*   turningBasePID(135);
    //driveInertial(24, 68, true);
    timeDrive(1400, 68, true);
    autonShoot1(2);
    wait(130, msec);
    driveInertial(47, 80, false); */

  // STEP 3
  // turningBasePID(189);
/*   timeDrive(500, 100, false);
  // driveInertial(40, 70, false);
  turningBasePID(225);
  timeDrive(1400, 90, true);
  taskHOLDa = vex::task(autonHoardone);
  turningBasePID(180);
  timeDrive(900, 100, true);
  wait(400, msec);
  autonShoot1(2);
  driveInertial(5, 90, false); */
}

void lastMinProgram() {
  // 1. Deploy
  lFront.setStopping(vex::brakeType::brake);
  rFront.setStopping(vex::brakeType::brake);
  lBack.setStopping(vex::brakeType::brake);
  rBack.setStopping(vex::brakeType::brake);

  lIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);

  wait(500, msec);
  lIntake.stop();
  rIntake.stop(); 
  wait(100, msec);

  // 2.
  lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  //hoardAll = thread(hoardAllRollers);
  driveInertial(23, 40, true);

  turningBasePID(135);
  //hoardAll.interrupt();
  lRoller.stop();
  rRoller.stop();
  lIntake.stop();
  rIntake.stop();
  timeDrive(1700, 60, true);
  
  //shootAll = thread(shootAllRollers);
  descoreTwoBalls();
  //shootAll.interrupt();

  // 3. Come to Center Goal &  Descore
  driveInertial(38, 40, false);
  
  turningBasePID(265);
  clearAllRollers();
  driveInertial(18, 40, true); ///// this is motion before 90 deg turn to center goal... dist can vary sometimes
  wait(150, msec);
  turningBasePID(0);
  driveInertial(10, 40, true);
  wait(300, msec);
  lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  for (int i = 1; i < 5; i++) {
    driveInertial(3, 30, true);
    //timeDrive(600, 30, true);
    wait(1000, msec);
    //timeDrive(500, 30, false);
    driveInertial(2, 30, false);
    wait(1000, msec);
  }
  lIntake.stop();
  rIntake.stop();

  // 4. Move to far corner goal to remove Completed Blue Rows
  driveInertial(10, 40, false);
  turningBasePID(0);
  driveInertial(9, 20, false);
  turningBasePID(320);
    lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  //driveInertial(113, 40, true);
  timeDrive(6900, 70, true);
  lIntake.stop();
  rIntake.stop();
  descoreTwoBalls();
  driveInertial(5, 50, false);


}

void evenmore() {
  lFront.setStopping(vex::brakeType::brake);
  rFront.setStopping(vex::brakeType::brake);
  lBack.setStopping(vex::brakeType::brake);
  rBack.setStopping(vex::brakeType::brake);

  lIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::fwd, 100, vex::velocityUnits::rpm);

  wait(500, msec);
  lIntake.stop();
  rIntake.stop(); 
  wait(100, msec);

  // 2.
  lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  //hoardAll = thread(hoardAllRollers);
  driveInertial(23, 40, true);

  turningBasePID(135);
  //hoardAll.interrupt();
  lRoller.stop();
  rRoller.stop();
  lIntake.stop();
  rIntake.stop();
  timeDrive(1700, 60, true);
  
  //shootAll = thread(shootAllRollers);
  descoreTwoBalls();
  driveInertial(38, 40, false);
  turningBasePID(347);
  clearAllRollers();
  lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  //driveInertial(84, 50, true);
  timeDrive(5000, 70, true);
  lIntake.stop();
  rIntake.stop();

  turningBasePID(0);
  descoreOneBall();

  driveInertial(10, 40, false);
  turningBasePID(215);
  clearAllRollers();
    lIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  rIntake.spin(vex::directionType::rev, 100, vex::velocityUnits::rpm);
  timeDrive(6000, 70, true);
  lIntake.stop();
  rIntake.stop();
  descoreTwoBalls();
  driveInertial(10, 40, false);

}
