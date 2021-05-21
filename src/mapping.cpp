#include "vex.h"

thread shoot;
thread hoard;
thread hoardTwo;
thread clean;

void rightAlliance() {
  // 15 second program starting on the right side
  thread setspeed(rollerControl);
  thread pos(getPosition);
  timer t;

  //deploy 
  deploy();
  setDrivetrainLock();
  wait(100, msec);
  //move to first goal
  //thread leftCustom(leftHomeGoalCustomLess); 
  setIntakeSpeed(-40);
  arcturn(1, 7, 325);
  //waitDrive();
  wait(250, msec);
  driveProfile(10, false);
  setIntakeSpeed(0);
 // leftCustom.interrupt();
  thread middleCustom(middleGoalCustomIntake);
  setPos(42, -4, false);
  wait(100, msec);
  arcturn(0, -6.5, 154);
  wait(200, msec);
  middleCustom.interrupt();
  setIntakeSpeed(50);
  driveProfile(22, true);
  setPos(1, -42, false);
  setIntakeSpeed(-65);
  //timeDrive(5, 100);
  //thread sideCustom(sideGoalCustom);
 // waitDrive();
  wait(400, msec);
  driveProfile (10, false);
  setIntakeSpeed(0);
  //drivetrainTurn(135);
  setPos(21, -42, false);
  wait(100, msec);
  thread middleCustomScore(middleGoalCustom);
  //arcturn(5, 7, 85);
  drivetrainTurn(90);
  timeDrive(6, 1000);
  //arcturnTime(5.2, 7.5, 1500);
  waitDrive();
  setIntakeSpeed(-8);
  driveProfile(10, false);
  setIntakeSpeed(-30);
  setPos(5, -102, false);
  timeDrive(4, 700);
  //waitDrive();
  Controller.Screen.clearLine();
  Controller.Screen.print(t.time(seconds));
}
                                                    
void leftAlliance() {
  // 15 second program starting on the right side
  thread setspeed(rollerControl);
  thread pos(getPosition);
  timer t;

  //deploy 
  deploy();
  setDrivetrainLock();
  wait(100, msec);
  //move to first goal
  //thread leftCustom(leftHomeGoalCustomLess); 
  setIntakeSpeed(-40);
  arcturn(1, 7, 325);
  //waitDrive();
  wait(250, msec);
  driveProfile(10, false);
  setIntakeSpeed(0);
 // leftCustom.interrupt();
  thread middleCustom(middleGoalCustomIntake);
  setPos(42, -1, false);
  wait(100, msec);
  arcturn(0, -6.5, 155);
  wait(200, msec);
  middleCustom.interrupt();
  setIntakeSpeed(60);
  driveProfile(22, true);
  wait(200, msec);
  timeDrive(-6, 300);
  setPos(1, -49, false);
  if(!getbottomLineInfo())
    setRollerCustom(-50);
  setIntakeSpeed(-65);
  //timeDrive(5, 100);
  //thread sideCustom(sideGoalCustom);
 // waitDrive();
  wait(400, msec);
  setRollerCustom(0);
  driveProfile (10, false);
  setIntakeSpeed(0);
  //drivetrainTurn(135);
  setPos(21, -44, false);
  wait(100, msec);
  thread middleCustomScore(middleGoalCustom);
  //arcturn(5, 7, 85);
  drivetrainTurn(90);
  timeDrive(6, 1000);
  //arcturnTime(5.2, 7.5, 1500);
  waitDrive();
  setIntakeSpeed(-8);
  driveProfile(10, false);
  setIntakeSpeed(-30);
  setPos(5, -110, false);
  timeDrive(4, 700);
  //waitDrive();
  Controller.Screen.clearLine();
  Controller.Screen.print(t.time(seconds));
  


  
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
  setPos(86, -15, false); 
  //go to goal
  setPos(100, -16, false);
  setHoardStopTrue();
  timeDrive(4, 500);
  //shoot and descore
  setShootStartTrue();
  rollerHold();
  driveProfile(10, false);
  drivetrainTurn(80);
  setCleanStartTrue();
  wait(350, msec);
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
  wait(200, msec);
  cornerThree.interrupt();
  
  //goal 7
  thread sideThree(shoot2Side);
  //go to first ball
  setPos(57, -55, false);
  //go to second ball
  setPos(53, -66, false);
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
  driveProfile(20, true);
  driveProfile(18, false);
  setHoardStopTrue();
  //go to goal
  setPos(0, -83, false);
  timeDrive(4, 800);
  //shoot and descore
  setShootStartTrue();
  rollerHold();
  setCleanStartTrue();
  arcturn(-7, -3, 180);
  cornerFour.interrupt();

  //center goal   
  thread middleGoal(shootMiddleGoal);
  timeDrive(-6, 1200);
  //go to first ball
  setPos(30, -24, false);
  drivetrainTurn(87);
  wait(50, msec);
  //descore center balls
  timeDrive(4, 1100);
  //shoot red ball 
  while(getBallsDetected() < 3)
    wait(50, msec);
  timeDrive(-6, 1000);

}

//test function for whatever needs sole tries
void test(){
  //thread setspeed(rollerControl);
  //thread test(shoot2Corner);
  drivetrainTurn(90);
  wait(100, msec);
  drivetrainTurn(0);
  wait(100, msec);
  drivetrainTurn(270);
    wait(100, msec);
  drivetrainTurn(315);
    wait(100, msec);
  drivetrainTurn(0);

}