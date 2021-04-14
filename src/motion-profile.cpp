#include "vex.h"

const double convertInches = (2.75 * M_PI) / 360;
const double convertDegrees = 360 / (2.75 * M_PI);
timer tim = timer();
struct Acceleration {
  double dist;
  bool fwd;
  double deg;
} acc;

double speed = 0;

int accelerate() {
  double lorig = encoderLeft.rotation(degrees) * convertInches;
 
  if (acc.fwd) {
    acc.dist += lorig;
    while ((encoderLeft.rotation(degrees) * convertInches) < acc.dist) {
      speed = 2.5 * exp(0.15 * (fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig)))  - 0.8;
      if (speed > 10)
        speed = 10;
      wait(10, msec);
    }
  } else {
    acc.dist = lorig - acc.dist;
    while ((encoderLeft.rotation(degrees) * convertInches) > acc.dist) {
      speed = 1.5 * exp(0.13 * ((fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig)))) ;
      if (speed > 10)
        speed = 10;
      wait(10, msec);
    }
  }
  return 0;
}

int accelerateturn() {
  Inertial.resetRotation();
  double turndeg = acc.deg;
  while ((Inertial.rotation()) < turndeg) {
    speed = 10 * exp(0.23 * (Inertial.heading()) - 1.5) ;
    if (speed > 10)
      speed = 10;
    wait(10, msec);
  }
  return 0;
}

int decelerate() {
  double lorig = encoderLeft.rotation(degrees) * convertInches;
  int n = 0;

  if (acc.fwd) {
    if (acc.dist < 14) {
      n = 14 - acc.dist;
    }
    acc.dist += lorig;
    while ((encoderLeft.rotation(degrees) * convertInches) < acc.dist) {
      speed = 1.1 * exp((-0.2 * fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig + n)) + 2) + 0.8 ;
        if (speed > 10) speed = 10;
        if (speed < 1.4) speed = 0;
        wait(10, msec);
    }
  } else {
    if (acc.dist < 14) {
      n = 14 - acc.dist;
    }
    acc.dist = lorig - acc.dist;
    while ((encoderLeft.rotation(degrees) * convertInches) > acc.dist) {
      speed = 1.1 * exp((-0.2 * fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig + n)) + 2) + 0.8; 
        if (speed > 10) speed = 10;
        if (speed < 1.4) speed = 0;
        wait(10, msec);
    }
  }
  return 0;
}
int decelerateturn() {
  Inertial.resetRotation();
  int n = 0;
  double turndeg = acc.deg;
  if (acc.deg < 5) {
    n = 5 - acc.deg;
  }
  while (Inertial.rotation() < turndeg) {
    speed = 1.1 * exp((-0.2 * Inertial.rotation()) + 2) + 0.8 ;
      if (speed > 10) speed = 10;
      if (speed < 1.4) speed = 0;
      wait(10, msec);
  }
  return 0;
}
int driveProfile(int dist, bool  fwd) {
  Inertial.resetRotation();
  double acceldist = 0;
  int targetL = 0;
  double actdist;

  if (dist > 28) 
  {
    acceldist = 14;
  } 
  else {
    acceldist = dist / 2.0;
  }
  // Switching units to degrees here
  dist *= convertDegrees;
 
  if (fwd) {
   actdist = encoderLeft.rotation(degrees) + dist;
  } else {
    actdist = encoderLeft.rotation(degrees) - dist;
  }

  if (fwd)  {
    acc.dist = acceldist; // feeding acc.dist inch value -- accelerate() handles inches
    acc.fwd = fwd;

    thread thread1(accelerate);

    // accelerate-- now we switch back to degrees!
    acceldist *= convertDegrees;
    targetL = encoderLeft.rotation(degrees) + acceldist; // target to finish acceleration
   
    while (encoderLeft.rotation(degrees) < targetL) {
     // printf("accelerate: %f\n", speed);

      lFront.spin(vex::directionType::fwd, speed, volt);
      lBack.spin(vex::directionType::fwd, speed, volt);
      rFront.spin(vex::directionType::fwd, speed, volt);
      rBack.spin(vex::directionType::fwd, speed, volt); 
      wait(10, msec);
    }
    thread1.interrupt();
    while (encoderLeft.rotation(degrees) < (actdist - acceldist)) {
      lFront.spin(vex::directionType::fwd, 10, volt);
      lBack.spin(vex::directionType::fwd, 10, volt);
      rFront.spin(vex::directionType::fwd, 10, volt);
      rBack.spin(vex::directionType::fwd, 10, volt); 
    } 
    acc.dist = acceldist * convertInches;
    thread thread3(decelerate);
    // decelerate
    targetL = encoderLeft.rotation(degrees) + acceldist;
    while (encoderLeft.rotation(degrees) < targetL ) {
     // printf("decelerate %f\n", speed);
      lFront.spin(vex::directionType::fwd, speed, volt);
      lBack.spin(vex::directionType::fwd, speed, volt);
      rFront.spin(vex::directionType::fwd, speed, volt);
      rBack.spin(vex::directionType::fwd, speed, volt);
      wait(10, msec);
    }
    thread3.interrupt();
  } 
  //backwards
  else {
    acc.dist = acceldist;
    acc.fwd = fwd;
    acceldist *= convertDegrees;
   
    thread thread2(accelerate);
    // accelerate
    targetL = encoderLeft.rotation(degrees) - acceldist;
   
    while ((encoderLeft.rotation(degrees) > targetL)) {
     // printf("accelerate: %f\n", speed);
      lFront.spin(vex::directionType::rev, speed, volt);
      lBack.spin(vex::directionType::rev, speed, volt);
      rFront.spin(vex::directionType::rev, speed, volt);
      rBack.spin(vex::directionType::rev, speed, volt);
      wait(10, msec);
    }
    thread2.interrupt();
   
    while (encoderLeft.rotation(degrees) > (actdist + acceldist)) {
      lFront.spin(vex::directionType::rev, 10, volt);
      lBack.spin(vex::directionType::rev, 10, volt);
      rFront.spin(vex::directionType::rev, 10, volt);
      rBack.spin(vex::directionType::rev, 10, volt);
    }


    acc.dist = acceldist * convertInches;
    //decelerate
    thread thread4(decelerate);
    targetL = encoderLeft.rotation(degrees) - acceldist;
    while (encoderLeft.rotation(degrees) > targetL) {
      //printf("decelerate %f\n", speed);
      lFront.spin(vex::directionType::rev, speed, volt);
      lBack.spin(vex::directionType::rev, speed, volt);
      rFront.spin(vex::directionType::rev, speed, volt);
      rBack.spin(vex::directionType::rev, speed, volt);
    } 
    thread4.interrupt();   
  }
  lFront.stop();
  lBack.stop();
  rFront.stop();
  rBack.stop();
  return 0;
}

void motionturn(double targetdeg) {
  Inertial.resetRotation();
  bool useright = true;
  double error = 3;
  tim.reset();
  while (fabs(targetdeg - Inertial.heading(degrees)) > 2 && tim.time(msec) < 2000) {
    // PID loop to determine motorPower at any given point in time
    double head = Inertial.heading(degrees);
    double errorright = targetdeg - head;
    double errorleft = fabs(targetdeg - head);
    double turnangle;
    if (targetdeg < head) {
      errorright = 360 - head + targetdeg;
    }
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
    if (error > 10) 
    {
      turnangle = 5;
    } 
    else {
      turnangle = error / 2.0;
    }

    wait(15, msec);

    // powering the motors
    if (useright) {
      acc.deg = turnangle;
      thread accel(accelerateturn);
      while (Inertial.rotation() < turnangle) {
        lBack.spin(vex::directionType::fwd, speed , volt);
        lFront.spin(vex::directionType::fwd, speed, volt);
        rBack.spin(vex::directionType::fwd, -speed, volt);
        rFront.spin(vex::directionType::fwd, -speed, volt);
      }
      accel.interrupt();
      while (Inertial.rotation() < error - turnangle) {
        lBack.spin(vex::directionType::fwd, 8.4 , volt);
        lFront.spin(vex::directionType::fwd, 8.4, volt);
        rBack.spin(vex::directionType::fwd, -8.4, volt);
        rFront.spin(vex::directionType::fwd, -8.4, volt);        
      }
      thread decel(accelerateturn);
      acc.deg = turnangle;
      while (Inertial.rotation() < error) {
        lBack.spin(vex::directionType::fwd, speed , volt);
        lFront.spin(vex::directionType::fwd, speed, volt);
        rBack.spin(vex::directionType::fwd, -speed, volt);
        rFront.spin(vex::directionType::fwd, -speed, volt);
      }
      decel.interrupt();
    }
    else {
      acc.deg = turnangle;
      thread accel(accelerateturn);
      while (Inertial.rotation() < turnangle) {
        lBack.spin(vex::directionType::fwd, -speed , volt);
        lFront.spin(vex::directionType::fwd, -speed, volt);
        rBack.spin(vex::directionType::fwd, speed, volt);
        rFront.spin(vex::directionType::fwd, speed, volt);
      }
      accel.interrupt();
      while (Inertial.rotation() < error - turnangle) {
        lBack.spin(vex::directionType::fwd, -8.4 , volt);
        lFront.spin(vex::directionType::fwd, -8.4, volt);
        rBack.spin(vex::directionType::fwd, 8.4, volt);
        rFront.spin(vex::directionType::fwd, 8.4, volt);        
      }
      thread decel(accelerateturn);
      acc.deg = turnangle;
      while (Inertial.rotation() < error) {
        lBack.spin(vex::directionType::fwd, -speed , volt);
        lFront.spin(vex::directionType::fwd, -speed, volt);
        rBack.spin(vex::directionType::fwd, speed, volt);
        rFront.spin(vex::directionType::fwd, speed, volt);
      }
      decel.interrupt();
    }
  }
  lFront.stop();
  lBack.stop();
  rFront.stop();
  rBack.stop();
}
