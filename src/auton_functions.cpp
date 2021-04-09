# include "vex.h"

void turningBasePID(double targetdeg) {
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

void inertialDrive(double dist, int speed, bool fwd) {
  // speed /= 8.33;
  // dist = dist * (360 / 20.25);
  Inertial.resetRotation();
  double inchessf = 360 / (2.75 * M_PI);
  dist *= inchessf;
  while (fabs(encoderRight.rotation(degrees)) < (fabs(encoderRight.rotation(degrees)) + dist) &&
         fabs(encoderLeft.rotation(degrees)) < (fabs(encoderLeft.rotation(degrees)) + dist)) {
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
/* int acceleratel() {
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
} */

/* int acceleratel() {
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
} */

