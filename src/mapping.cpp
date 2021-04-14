#include "vex.h"

thread shoot;
thread hoard;
thread clean;

void leftAlliance() {
  // 15 second program starting on the left side
}

void rightAlliance() {
  // 15 second program starting on the right side
}

void skills() {
  // 60 second program
  lFront.setStopping(brake);
  lBack.setStopping(brake);
  rBack.setStopping(brake);
  rFront.setStopping(brake);


  thread setspeed(rollerControl);
  thread pos(getPosition);
  //push ball back
   turningBasePID(28);
  lFront.spin(fwd, -65, pct);
  rFront.spin(fwd, -65, pct);
  lBack.spin(fwd, -65, pct);
  rBack.spin(fwd, -65, pct);
  wait(400, msec);
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();

  //goal1

   hoard = thread(ahoardAllRollers);
  setPos(6, 16);
  setPos(13, 40);
  driveProfile(15, false);
  setPos(-3, 43);

  lFront.spin(fwd, 1.9, volt);
  rFront.spin(fwd, 1.9, volt);
  lBack.spin(fwd, 1.9, volt);
  rBack.spin(fwd, 1.9, volt);
  wait(800, msec);
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();

  shoot = thread(shootAllRollers);
  adescoreTwoBalls();
 
  driveProfile(20, false);
  acleanBalls();

  wait(50, msec);
  //goal 2
  hoard = thread(ahoardAllRollers);
  setPos(50, 8);
  setPos(48, 36); 
  wait(50, msec);
  //setPos(48, 40);
  lFront.spin(fwd, 2, volt);
  rFront.spin(fwd, 2, volt);
  lBack.spin(fwd, 2, volt);
  rBack.spin(fwd, 2, volt);
  wait(800, msec);
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();

  shoot = thread(shootAllRollers);
  wait(40, msec);
  adescoreOneBall();
  wait(20, msec);
  driveProfile(20, false);
  turningBasePID(340);
  acleanBalls();   

  //goal 3
  hoard = thread(ahoardAllRollers);
  setPos(84, 46);
  driveProfile(17, false);
  setPos(95, 27);
  setPos(127, 63);

  lFront.spin(fwd, 1.9, volt);
  rFront.spin(fwd, 1.9, volt);
  lBack.spin(fwd, 1.9, volt);
  rBack.spin(fwd, 1.9, volt);
  wait(800, msec);
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();
  shoot = thread(shootAllRollers);
  adescoreTwoBalls();
 
  driveProfile(20, false);
  turningBasePID(345);
  acleanBalls();
}
