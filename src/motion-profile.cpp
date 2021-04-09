# include "vex.h"

const double convertInches = (2.75 * M_PI) / 360;
const double convertDegrees = 360 / (2.75 * M_PI);

struct Acceleration {
  double dist;
  bool speedup;
  bool fwd;
} acc;

int speedr = 0;
int speedl = 0;

int accelerater() {
  //acc.dist *= 360 / (2.75 * M_PI);
  acc.dist += (encoderLeft.rotation(degrees) * convertInches);
  double rorig = fabs(encoderRight.rotation(degrees)) * convertInches;
  
  if (acc.speedup) {
    while ((fabs(encoderRight.rotation(degrees)) * convertInches) < acc.dist) {
      speedr = (4.410714286 * pow((fabs(encoderRight.rotation(degrees)) * convertInches) - rorig, 2)) - (24.32738095 * ((fabs(encoderRight.rotation(degrees)) * convertInches) - rorig)) + 28.875; //quadratic
      speedr = pow(2, ((fabs(encoderRight.rotation(degrees)) * convertInches) - rorig));
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
    while ((fabs(encoderRight.rotation(degrees)) * convertInches) < acc.dist) {
      //speedr = (4.410714286 * pow((fabs(encoderRight.rotation(degrees)) * convertInches) - rorig, 2)) - (55.06547619 * ((fabs(encoderRight.rotation(degrees)) * convertinches) - rorig)) + 167.1964286; //quadratic
      speedr = 256 * pow(0.5, (((fabs(encoderRight.rotation(degrees))) * convertInches) - rorig) + n); //exponential
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
  dist *= convertDegrees;
  double actdist = fabs(encoderRight.rotation(degrees)) + dist;
  if (fwd) {
    acc.dist = acceldist;
    acc.speedup = true;
    acc.fwd = fwd;
  
    acceldist *= convertDegrees;

    thread thread1(accelerater); 

    //accelerate
    int targetL = fabs(encoderLeft.rotation(degrees)) + acceldist;
    int targetR = fabs(encoderRight.rotation(degrees)) + acceldist;
    while ((fabs(encoderLeft.rotation(degrees)) < targetL) && 
      fabs(encoderRight.rotation(degrees)) < targetR) {
/*    Brain.Screen.clearScreen();
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
