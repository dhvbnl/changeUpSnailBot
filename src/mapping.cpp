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
  turningBasePID(25);
  lFront.spin(fwd, -60, pct);
  rFront.spin(fwd, -60, pct);
  lBack.spin(fwd, -60, pct);
  rBack.spin(fwd, -60, pct);
  wait(400, msec);
  lFront.stop();
  rFront.stop();
  lBack.stop();
  rBack.stop();

  //get ball1

  hoard = thread(ahoardAllRollers);
  setPos(6, 16);
  setPos(13, 40);
  driveProfile(15, false);
  setPos(-4, 45);

  shoot = thread(shootAllRollers);
  adescoreTwoBalls();

  driveProfile(20, false);
  clean = thread(acleanBalls);

}
