#include "vex.h"

thread shoot;
thread hoard;
thread hoardTwo;
thread clean;
int hi = 2;

void leftAlliance() {
  // 15 second program starting on the left side
  //arc
  
  thread setspeed(rollerControl);
  thread pos(getPosition);

  deploy();
  setDrivetrainLock();
  arcturn(7, 0, 110);
  hoard = thread(shootThreeRemoveOne);
  timeDrive(5, 600);
  while(getRollerSpeed() > 10) {
    wait(50, msec);
  }
  driveProfile(10, false);
  hoard.interrupt();
  clean = thread(acleanBalls);
  arcturn(-6, -3, 90);
  arcturn(-1.6, -5, 178);
  //timeDrive(-5, 500);
  shoot = thread(shootOneRemoveTwo);
  timeDrive(6, 1500);
  while(getRollerSpeed() > 10) {
    wait(50, msec);
  }
  clean = thread(acleanBalls);
  timeDrive(-6, 1200);
  wait(100, msec);
  hoardTwo = thread(shootTwoRemoveTwo);
  //drivetrainTurn(230);
  setPos(-82, -6, false);
  timeDrive(5, 700);
  while(getRollerSpeed() > 10) {
    wait(50, msec);
  }
  wait(200, msec);
  driveProfile(20, false);
 //timeDrive(-12, 700);
  //timeDrive(6, 2000);
  
  
  

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
  timeDrive(-6.5, 400);

  // goal2

  thread cornerOne(shoot2Corner);
  //setPos(6, 16);
  setPos(16, 44, false);
  driveProfile(15, false);
  setHoardStopTrue();
  setPos(-1, 45, false);
  timeDrive(3, 550);
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  driveProfile(20, false);
  wait(700, msec);
  cornerOne.interrupt();
  // goal 3
  thread sideOne(shoot2Side);
  //get first ball
  setPos(50.5, 11, false);
  setPos(52, 33, false);
  //get second ball
  wait(200, msec);
  setHoardStopTrue();
  //go to goal and shoot
  timeDrive(6, 800);
  //wait(800, msec);
  setShootStartTrue();
  rollerHold();
  
  //move back from goal and clean
  driveProfile(20, false);
  wait(100, msec);
  setCleanStartTrue();
  drivetrainTurn(330);
  wait(500, msec);
  sideOne.interrupt();
  wait(50, msec);

  // goal 4
  thread CornerTwo(shoot2Corner);
  setPos(97.5, 28, false);
  //drivetrainTurn(45);
  
  //driveProfile(40, true);
  //timeDrive(7, 1500);
  driveProfile(7, false);
  drivetrainTurn(0);
  timeDrive(6, 900);
  //setPos(84, 40, false);
  driveProfile(18, false);
  setHoardStopTrue();
  setPos(110, 44, false);
  timeDrive(1.9, 800); 
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  driveProfile(25, false);

  //start from goal 4
  //setPos(110, 41);
/*   drivetrainTurn(63);
  timeDrive(7, 2500);
  drivetrainTurn(61);
  timeDrive(5, 1500);
  printPos();
  wait(50, msec);

  driveProfile(20, false); */
  
  drivetrainTurn(195);
  CornerTwo.interrupt();
  thread sideTwo(shoot1Side);
  driveProfile(40, true);
  drivetrainTurn(90);
  driveProfile(30, true);
  setHoardStopTrue();
  timeDrive(6, 400);
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  timeDrive(-6, 1000);



  //setPos(65, -4, false); 

}

void test(){
  thread setspeed(rollerControl);

  thread test(shoot2Corner);
  wait(3, sec);
  setHoardStopTrue();
  wait(2, sec);
  setShootStartTrue();
  wait(3, sec);
  setCleanStartTrue();
}
