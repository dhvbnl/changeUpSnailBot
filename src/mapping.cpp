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
  setDrivetrainLock();

  thread setspeed(rollerControl);
  thread pos(getPosition);

  // push ball back
  drivetrainTurn(24);
  timeDrive(-5.5, 400);

  // goal1

  hoard = thread(ahoardAllRollers);
  //setPos(6, 16);
  setPos(16, 43);
  driveProfile(15, false);
  setPos(-3, 43);
  timeDrive(1.8, 800);

  shoot = thread(shootAllRollers);
  adescoreTwoBalls();
  wait(60, msec);
  clean = thread(acleanBalls);
  driveProfile(20, false);
  wait(350, msec);
  
  
  // goal 2
  hoard = thread(ahoardAllRollers);
  //get first ball
  setPos(51, 6);
  //get second ball
  setPos(51, 36);
  wait(300, msec);
  hoard.interrupt();
  //go to goal and shoot
  timeDrive(3, 1000);
  shoot = thread(shootAllRollers);
  wait(60, msec);
  adescoreOneBall();
  wait(60, msec);
  clean = thread(acleanBalls);
  //move back from goal and clean
  driveProfile(25, false);
  wait(50, msec);
  clean.interrupt();
  

  // goal 3
  hoard = thread(ahoardAllRollers);
  setPos(86, 44);
  driveProfile(17, false);
  setPos(97, 25);
  setPos(110, 41);

  timeDrive(1.9, 800);
  shoot = thread(shootAllRollers);
  adescoreTwoBalls();

  driveProfile(10, false);
  clean = thread(acleanBalls);
  wait(50, msec);
}

void test(){
  setDrivetrainLock();
  drivetrainTurn(45);
  Brain.Screen.print("KSOAIDFHOIHFOS");
  wait(500, msec);
  
/*   drivetrainTurn(180);
  wait(500, msec);
  drivetrainTurn(270);
  wait(500, msec);
  drivetrainTurn(0); */
}
