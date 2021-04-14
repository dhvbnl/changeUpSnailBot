#include "vex.h"
#include "motion-profile.h"
timer t = timer();
void turningBasePID(double targetdeg) {
  //Controller.Screen.print(" target deg: %f", targetdeg);
  // proportionality constants
  double kP = 0.4;
  double kI = 0.00001;
  double kD = 0.6;
  
  // PID loop variables
  double error = 3;
  double integral = 0;
  double derivative = 0;
  double prevError = 0;
  double motorPower = 0;
  bool useright = true;
  t.reset();
  while (fabs(targetdeg - Inertial.heading(degrees)) > 2 && t.time(msec) < 2000) {
    // PID loop to determine motorPower at any given point in time
    double head = Inertial.heading(degrees);
    double errorright = targetdeg - head;
    if (targetdeg < head) {
      errorright = 360 - head + targetdeg;
    }
    double errorleft = fabs(targetdeg - head);
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
    if (useright) {
      lBack.spin(vex::directionType::fwd, motorPower , vex::velocityUnits::pct);
      lFront.spin(vex::directionType::fwd, motorPower, vex::velocityUnits::pct);
      rBack.spin(vex::directionType::fwd, -motorPower, vex::velocityUnits::pct);
      rFront.spin(vex::directionType::fwd, -motorPower,
                  vex::velocityUnits::pct);
    }
    else {
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

