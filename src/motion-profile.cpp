#include "vex.h"

const double convertInches = (2.75 * M_PI) / 360;
const double convertDegrees = 360 / (2.75 * M_PI);

struct Acceleration {
  double dist;
  bool fwd;
} acc;

double speed = 0;

int accelerate() {
  double lorig = encoderLeft.rotation(degrees) * convertInches;
 
  if (acc.fwd) {
    acc.dist += lorig;
    while ((encoderLeft.rotation(degrees) * convertInches) < acc.dist) {
      speed = 1.2 * exp(0.1 * (fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig)))  + 0.5;
      if (speed > 8.4)
        speed = 8.4;
      wait(10, msec);
    }
  } else {
    acc.dist = lorig - acc.dist;
    while ((encoderLeft.rotation(degrees) * convertInches) > acc.dist) {
      speed = 1.2 * exp(0.1 * ((fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig)))) + 0.5;
      if (speed > 8.4)
        speed = 8.4;
      wait(10, msec);
    }
  }
  return 0;
}

int decelerate() {
  double lorig = encoderLeft.rotation(degrees) * convertInches;
  int n = 0;

  if (acc.fwd) {
    if (acc.dist < 20) {
      n = 20 - acc.dist;
    }
    acc.dist += lorig;
    while ((encoderLeft.rotation(degrees) * convertInches) < acc.dist) {
      speed = 1.2 * exp((-0.1 * fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig + n)) + 2) - 0.5;
        if (speed > 8.4) speed = 8.4;
        if (speed < 1.4) speed = 0;
        wait(10, msec);
    }
  } else {
    if (acc.dist < 20) {
      n = 20 - acc.dist;
    }
    acc.dist = lorig - acc.dist;
    while ((encoderLeft.rotation(degrees) * convertInches) > acc.dist) {
      speed = 1.2 * exp((-0.1 * fabs((fabs(encoderLeft.rotation(degrees)) * convertInches) - lorig + n)) + 2) - 0.5; 
        if (speed > 8.4) speed = 8.4;
        if (speed < 1.4) speed = 0;
        wait(10, msec);
    }
  }
  return 0;
}

int driveProfile(int dist, bool  fwd) {
  Inertial.resetRotation();
  int acceldist = 0;
  int targetL = 0;
  double actdist;

  if (dist > 40) 
  {
    acceldist = 20;
  } 
  else {
    acceldist = dist / 2;
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
      printf("accelerate: %f\n", speed);

      lFront.spin(vex::directionType::fwd, speed, volt);
      lBack.spin(vex::directionType::fwd, speed, volt);
      rFront.spin(vex::directionType::fwd, speed, volt);
      rBack.spin(vex::directionType::fwd, speed, volt); 
      wait(10, msec);
    }
    thread1.interrupt();
    while (encoderLeft.rotation(degrees) < (actdist - acceldist)) {
      lFront.spin(vex::directionType::fwd, 8.4, volt);
      lBack.spin(vex::directionType::fwd, 8.4, volt);
      rFront.spin(vex::directionType::fwd, 8.4, volt);
      rBack.spin(vex::directionType::fwd, 8.4, volt); 
    } 
    acc.dist = acceldist;
    thread thread3(decelerate);
    // decelerate
    targetL = encoderLeft.rotation(degrees) + acceldist;
    while (encoderLeft.rotation(degrees) < targetL ) {
      printf("decelerate %f\n", speed);
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
      printf("accelerate: %f\n", speed);
      lFront.spin(vex::directionType::rev, speed, volt);
      lBack.spin(vex::directionType::rev, speed, volt);
      rFront.spin(vex::directionType::rev, speed, volt);
      rBack.spin(vex::directionType::rev, speed, volt);
      wait(10, msec);
    }
    thread2.interrupt();
   
    while (encoderLeft.rotation(degrees) > (actdist + acceldist)) {
      lFront.spin(vex::directionType::rev, 8.4, volt);
      lBack.spin(vex::directionType::rev, 8.4, volt);
      rFront.spin(vex::directionType::rev, 8.4, volt);
      rBack.spin(vex::directionType::rev, 8.4, volt);
    }


    acc.dist = acceldist;
    //decelerate
    thread thread4(decelerate);


    targetL = encoderLeft.rotation(degrees) - acceldist;
    while (encoderLeft.rotation(degrees) > targetL) {
      printf("decelerate %f\n", speed);
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