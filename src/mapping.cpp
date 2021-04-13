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
  //setPos(0, 20);
  setPos(0,20);
/*   hoard = thread(ahoardAllRollers);
  //setPos(0, 28); //move to place to turn to second ball
  driveProfile(28, true);
  intake(75);
  setPos(-26, 28);//get second ball
  intake(0);
  //hoard.interrupt();
  driveProfile(18, false);
  setPos(-30, 9);
  shoot = thread(shootAllRollers);
  adescoreTwoBalls();  */

}
