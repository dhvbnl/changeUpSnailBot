#include "vex.h"

thread shoot;
thread hoard;
thread hoardTwo;
thread clean;

void rightAlliance() {
  // 15 second program starting on both sides
  //threads to check odometry and for controller roller speeds
  thread setspeed(rollerControl);
  thread pos(getPosition);

  //deploy 
  deploy();
  setDrivetrainLock();
  //move to first goal 
  arcturn(7, 0, 110);
  hoard = thread(shootThreeRemoveOne);
  timeDrive(5, 600);
  while(getRollerSpeed() > 10) {
    wait(50, msec);
  }
  driveProfile(10, false);
  hoard.interrupt();
  clean = thread(acleanBalls);
  //"S" turn to the second goal
  arcturn(-6, -3, 90);
  arcturn(-1.6, -5, 178);
  shoot = thread(shootOneRemoveTwo);
  timeDrive(6, 1500);
  while(getRollerSpeed() > 10) {
    wait(50, msec);
  }
  clean = thread(acleanBalls);
  timeDrive(-6, 1200);
  wait(100, msec);
  hoardTwo = thread(shootTwoRemoveTwo);
  //set position to third goal
  setPos(-82, -6, false);
  timeDrive(5, 700);
  while(getRollerSpeed() > 10) {
    wait(50, msec);
  }
  wait(200, msec);
  driveProfile(20, false);

}
                                                    
void leftAlliance() {
  // 15 second program starting on the right side
  
  
}

void skills() {
  // 60 second program
  setDrivetrainLock();
  thread setspeed(rollerControl);
  thread pos(getPosition);

  deploy();

   // push ball back to goal 1
  drivetrainTurn(24);
  timeDrive(-6, 400);

  // goal2
  thread cornerOne(shoot2Corner);
  //get first ball
  setPos(16, 44, false);
  driveProfile(15, false);
  setHoardStopTrue();
  //get second ball
  setPos(-1, 45, false);
  timeDrive(3, 550);
  //shoot balls
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  //move back from goals and clean
  driveProfile(20, false);
  wait(500, msec);
  cornerOne.interrupt();

  // goal 3
  thread sideOne(shoot2Side);
  //get first ball
  setPos(50.5, 12, false);
  setPos(52, 31, false);
  //get second ball
  wait(400, msec);
  setHoardStopTrue();
  //go to goal and shoot
  timeDrive(4, 600);
  //wait(800, msec);
  setShootStartTrue();
  rollerHold();
  
  //move back from goal and clean
  driveProfile(20, false);
  wait(100, msec);
  setCleanStartTrue();
  drivetrainTurn(330);
  wait(200, msec);
  sideOne.interrupt();

  // goal 4
  thread CornerTwo(shoot2Corner);
  //go to first ball
  setPos(97.5, 25, false);
  wait(100, msec);
  driveProfile(5, false);
  //go to second ball
  drivetrainTurn(1);
  driveProfile(21, true);
  driveProfile(15, false);
  setHoardStopTrue();
  //go to goal
  setPos(107, 44, false);
  timeDrive(4, 600); 
  //shoot balls and descore
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  driveProfile(25, false);
  CornerTwo.interrupt();

  //goal 5
  thread sideTwo(shoot1Side);
  //go to first ball
  setPos(86, -14, false); 
  //go to goal
  setPos(100, -19, false);
  setHoardStopTrue();
  timeDrive(4, 500);
  //shoot and descore
  setShootStartTrue();
  rollerHold();
  driveProfile(10, false);
  setCleanStartTrue();
  drivetrainTurn(80);
  wait(200, msec);
  sideTwo.interrupt();

  //goal 6
  thread cornerThree(shoot2Corner);
  //go to first ball
  setPos(105, -48, false);
  setPos(93, -75, false);
  //go to second ball
  driveProfile(15, false);
  setHoardStopTrue();
  //go to goal
  setPos(109, -79, false);
  timeDrive(4, 400);
  //shoot and descore
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  driveProfile(20, false);
  cornerThree.interrupt();
  
  //goal 7
  thread sideThree(shoot2Side);
  //go to first ball
  setPos(57, -55, false);
  //go to second ball
  setPos(54, -66, false);
  wait(400, msec);
  //go to goal
  setHoardStopTrue();
  timeDrive(4, 600);
  //shoot and descore
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  driveProfile(10, false);
  wait(50, msec);
  sideThree.interrupt();
  wait(100, msec);
  
  //goal 8
  thread cornerFour(shoot2Corner);
  //go to first ball
  setPos(14, -65, false);
  wait(100, msec);
  driveProfile(5, false);
  //turn and go to second ball
  drivetrainTurn(200);
  driveProfile(21, true);
  driveProfile(18, false);
  setHoardStopTrue();
  //go to goal
  setPos(-1, -84, false);
  timeDrive(4, 600);
  //shoot and descore
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  arcturn(-7, -3, 180);
  cornerFour.interrupt();

  //center goal   
  thread middleGoal(shootMiddleGoal);
  timeDrive(-4, 1200);
  //go to first ball
  setPos(30, -20, false);
  drivetrainTurn(90);
  setHoardStopTrue();
  wait(50, msec);
  setIntakeSpeed(-100);
  //descore center balls
  timeDrive(4, 900);
  timeDrive(-4, 550);
  timeDrive(4, 550);
  timeDrive(-4, 550);
  timeDrive(4, 550);
  timeDrive(-4, 550);
  timeDrive(5, 800);
  setIntakeSpeed(0);
  //shoot red ball 
  setShootStartTrue();
  rollerHold();
  timeDrive(-6, 600);

}

//test function for whatever needs sole tries
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
