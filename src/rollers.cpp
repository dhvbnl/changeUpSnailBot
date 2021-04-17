#include "vex.h"

//global rollerSpeed checker
int rollerSpeed = 0;
//global descore checker
int shootState = 2;

//global auton roller variables
bool hoardstop = false;
bool shootstart = false;
bool shootdone = false;
bool cleanstart = false;
bool descoreState = false;
//time for shoot exepction handling
timer alignRollers = timer();


//macro threads;
thread hoardAll;
thread hoardReset;
thread shootAll;
thread shootOne;
thread shootTwo;
thread cleanAll;
thread descoreOne;
thread descoreTwo;

int rollerControl() {
  while (true) {
    runMacros();
    setRollerSpeed();
    wait(10, msec);
  }
}

// setters

// sets speed for both rollers based on global speed
void setRollerSpeed() {
  lRoller.spin(fwd, rollerSpeed, pct);
  rRoller.spin(fwd, rollerSpeed, pct);
}
// sets speed for both rollers based on one speed parameter

void setRollerCreep() {
  rRoller.setBrake(coast);
  lRoller.setBrake(coast);
}

void setRollerLock() {
  rRoller.setBrake(brake);
  lRoller.setBrake(brake);
}

//flags for autonRoller
void setHoardStopTrue(){ hoardstop = true; };
void setHoardStopFalse(){ hoardstop = false; };
void setShootStartTrue(){ shootstart = true; };
void setShootStartFalse(){ shootstart = false; };
void setCleanStartTrue(){ cleanstart = true; };
void setCleanStartFalse(){ cleanstart = false; };


// getters

int getLRollerSpeed() { return lRoller.velocity(pct); };
int getRRollerSpeed() { return rRoller.velocity(pct); };
int getRollerSpeed() { return rollerSpeed; };

int getLRollerTemp() { return lRoller.temperature(celsius); };
int getRRollerTemp() { return rRoller.temperature(celsius); };

//gets if ball is present in a specific location in the rollers
bool gettopLineInfo() { return lineTop.value(pct) < lineMax; };
bool getmiddleLineInfo() { return lineMiddle.value(pct) < lineMax; };
bool getbottomLineInfo() { return lineBottom.value(pct) < lineMax; };

bool getDescoreState() { return descoreState; }; 

int getBallsDetected() {
  int ballCount = 0;
  if (gettopLineInfo())
    ballCount++;
  if (getmiddleLineInfo())
    ballCount++;
  if (getbottomLineInfo())
    ballCount++;
  return ballCount;
}

// control

// checks controller buttons and runs macros based on selection
void runMacros() {
  if (getL1Pos()) {
    stopAllMacros();
    shootAll = thread(shootAllRollers);
    descoreChooser();
  } else if (getRightPos()) {
    stopAllMacros();
    if(shootState == 2)
      shootState = 0;
    shootOne = thread(shootOneRollers);
    descoreChooser();
  } else if (getYPos()) {
    stopAllMacros();
    shootTwo = thread(shootTwoRollers);
  } else if (getL2Pos()) {
    stopAllMacros();
    hoardAll = thread(hoardAllRollers);
  } else if(getUpPos()){
    stopAllMacros();
    hoardReset = thread(resetForHoard);
  } else if (getR1Pos()) {
    stopAllMacros();
    cleanAll = thread(clearAllRollers);
  } else if (getR2Pos()) {
    stopAllMacros();
    rollerSpeed = 0;
    playIntake();
  } else if (getAPos())
    shootState = 2;
  else if (getBPos())
    shootState = 1;
  else if (getXPos())
    shootState = 0;
}

//stops all macros in order to control tasks running
void stopAllMacros() {
  shootAll.interrupt();
  shootOne.interrupt();
  hoardAll.interrupt();
  descoreTwo.interrupt();
  descoreOne.interrupt();
}

//picking which descore to run based on selection
void descoreChooser() {
  if(shootState == 2)
    descoreTwo = thread(descoreTwoBalls);
  else if(shootState == 1)
    descoreOne = thread(descoreOneBall);
}

// shoots all balls from rollers
int shootAllRollers() {
  if (getbottomLineInfo()) {
    rollerSpeed = 100;
    while (getbottomLineInfo())
      wait(50, msec);
    while (!getmiddleLineInfo())
      wait(50, msec);
    shootAllRollers();
  } else if (getmiddleLineInfo()) {
    rollerSpeed = 100;
    while (getmiddleLineInfo())
      wait(50, msec);
    while (!gettopLineInfo())
      wait(50, msec);
    shootAllRollers();
  } else if (gettopLineInfo()) {
    rollerSpeed = 100;
    while (gettopLineInfo())
      wait(50, msec);
  } else {
    rollerSpeed = 30;
    alignRollers.clear();
    while (getBallsDetected() == 0 && alignRollers.time(msec) < 300)
      wait(50, msec);
    rollerSpeed = 0;
  }
  wait(250, msec);
  rollerSpeed = 0;
  return 0;
}

//shoots only one ball for match play
int shootOneRollers() {
  if(gettopLineInfo()){
    rollerSpeed = 100;
    while(gettopLineInfo())
      wait(50, msec);
  }
  else if(getmiddleLineInfo()){
    rollerSpeed = 100;
    while(!gettopLineInfo())
      wait(50, msec);
    while(gettopLineInfo())
      wait(50, msec);
    
  }
  else if(getbottomLineInfo()){
    rollerSpeed = 100;
    while(!gettopLineInfo())
      wait(50, msec);
    while(gettopLineInfo())
      wait(50, msec);
  } else {
    rollerSpeed = 30;
    alignRollers.clear();
    while (getBallsDetected() == 0 && alignRollers.time(msec) < 300)
      wait(50, msec);
    rollerSpeed = 0;
  }
  wait(50, msec);
  rollerSpeed = 0;
  return 0;
}

//shoots only two balls for match play
int shootTwoRollers(){
  if(gettopLineInfo() && getmiddleLineInfo()){
    rollerSpeed = 100;
    while(getmiddleLineInfo())
      wait(50, msec);
    while(gettopLineInfo())
      wait(50, msec);
    while(!gettopLineInfo())
      wait(50, msec);
    while(gettopLineInfo())
      wait(50, msec);
  }
  else if(getmiddleLineInfo() && getbottomLineInfo()){
    rollerSpeed = 100;
    while(getbottomLineInfo())
      wait(50, msec);
    while(getmiddleLineInfo())
      wait(50, msec);
    while(!getmiddleLineInfo())
      wait(50, msec);
    while(getmiddleLineInfo())
      wait(50, msec);
    while(gettopLineInfo())
      wait(50, msec);
    while(!gettopLineInfo())
      wait(50, msec);
    while(gettopLineInfo())
      wait(50, msec);
  } else {
    rollerSpeed = 30;
    alignRollers.clear();
    while (getBallsDetected() == 0 && alignRollers.time(msec) < 300)
      wait(50, msec);
    rollerSpeed = 0;
  }
  wait(50, msec);
  rollerSpeed = 0;
  return 0;
}

//hoard balls to collect from the field
int hoardAllRollers() {
  resetForHoard();
  while (!getbottomLineInfo() && !getmiddleLineInfo())
    wait(50, msec);
  rollerSpeed = 50;
  while (!getmiddleLineInfo())
    wait(50, msec);
  rollerSpeed = 0;
  while (!getbottomLineInfo())
    wait(50, msec);
  rollerSpeed = 30;
  while (!gettopLineInfo())
    wait(10, msec);
  wait(50, msec);
  rollerSpeed = 0;
  return 0;
}

//fixes ball positions if they are not next to the correct sensors
int resetForHoard() {
  if(gettopLineInfo() && !getmiddleLineInfo()) {
    rollerSpeed = -20;
    while(!getmiddleLineInfo())
      wait(50, msec);
    rollerSpeed = 0;
  }
  return 0;
}

//remove all balls to get rid of other coller
int clearAllRollers() {
  pauseIntake();
  setIntakeSpeed(-75);
  rollerSpeed = -100;
  wait(500, msec);
  rollerSpeed = 0;
  playIntake();
  return 0;
}

//remove one ball from goal
int descoreOneBall() {
  if (getBallsDetected() > 0) {
    wait(300, msec);
    pauseIntake();
    setIntakeSpeed(75);
    while (getbottomLineInfo())
      wait(50, msec);
    while (!getbottomLineInfo())
      wait(50, msec);
    setIntakeSpeed(-100);
    wait(200, msec);
    playIntake();
  }
  return 0;
}

// remove two balls from goal
int descoreTwoBalls() {
  if (getBallsDetected() > 0) {
    wait(100, msec);
    pauseIntake();
    setIntakeSpeed(75);
    while (getbottomLineInfo())
      wait(50, msec);
    while(getmiddleLineInfo())
      wait(50, msec);
    while (!getbottomLineInfo())
      wait(50, msec);
      if(rollerSpeed < 50)
        rollerSpeed = 100;
    while (!getmiddleLineInfo())
      wait(50, msec);
      if(rollerSpeed < 50)
        rollerSpeed = 100;
    rollerSpeed = 0;
    while(!getbottomLineInfo())
      wait(50, msec);
    setIntakeSpeed(-100);
    wait(200, msec);
    playIntake();
  }
  return 0;
}

//auton roller macros

//flexible function in order to get balls and remove from the goal for the autonomous program
//uses flags that can be controlled from mapping so robot does not get stuck anywhere
int autonRoller(int hoardBalls, int descoreBalls){
  setHoardStopFalse();
  setShootStartFalse();
  setCleanStartFalse();
  shootdone = false;

  for(int i = 0; i < hoardBalls; i++){
    rollerSpeed = 0;
    setIntakeSpeed(100);
    while(!getbottomLineInfo()){
      if(shootstart || hoardstop)
        break;
      wait(50, msec);
    }
    if(shootstart || hoardstop)
      break;
    if(!gettopLineInfo()){
      rollerSpeed = 20;
      while(getmiddleLineInfo())
        wait(50, msec);
      while(!getmiddleLineInfo())
        wait(50, msec);
      rollerSpeed = 0;
    }
  }
  setIntakeSpeed(0);
  while(!shootstart)
    wait(50, msec);
  rollerSpeed = 100;
  while(getbottomLineInfo())
    wait(50, msec);
  if(descoreBalls == 2)
    thread descoreTwo(adescoreTwoBalls);
  else if(descoreBalls == 1)
    thread descoreOne(adescoreOneBall);
  while(!getmiddleLineInfo())
    wait(50, msec);
  while(getmiddleLineInfo())
    wait(50, msec);
  while(!gettopLineInfo())
    wait(50, msec);
  while(gettopLineInfo())
    wait(50, msec);
  wait(100, msec);
  rollerSpeed = 0;
  while(!descoreState)
    wait(50, msec);
  wait(100, msec);
  shootdone = true;
  rollerSpeed = 0;
  setIntakeSpeed(-40);
  wait(200, msec);
  setIntakeSpeed(0);
  while(!cleanstart)
    wait(50, msec);
  rollerSpeed = -100;
  setIntakeSpeed(-80);
  wait(1000, msec);
  rollerSpeed = 0;
  setIntakeSpeed(0);
  return 0;
}

//function with parameters controlled to work in a thread
int shoot2Corner(){
  autonRoller(2, 2);
  return 0;
}

//function with parameters controlled to work in a thread
int shoot1Corner(){
  autonRoller(1, 2);
  return 0;
}

//function with parameters controlled to work in a thread
int shoot2Side(){
  autonRoller(2, 1);
  return 0;
}

//function with parameters controlled to work in a thread
int shoot1Side(){
  autonRoller(1, 1);
  return 0;
}

//function with parameters controlled to work in a thread
int shootMiddleGoal(){
  autonRoller(1, 0);
  return 0;
}

//descore one ball from goal edited for autonomous
int adescoreOneBall() {
  descoreState = false;
  wait(150, msec);
    setIntakeSpeed(60);
    while (getbottomLineInfo())
      wait(50, msec);
    while(!getbottomLineInfo())
      wait(50, msec);
    setIntakeSpeed(0);
  descoreState = true;
  return 0;
}

//descore one ball from goal edited for autonomous
int adescoreTwoBalls() {
  descoreState = false;
  wait(150, msec);
    setIntakeSpeed(60);
    while (!getbottomLineInfo()){
      wait(50, msec);
    }
    while (getmiddleLineInfo()){
      wait(50, msec);
    }
    while(!getmiddleLineInfo()){
      runRollersBack();
      wait(50, msec);
    }
    while(!getbottomLineInfo())
      wait(50, msec);
    setIntakeSpeed(0);
  descoreState = true;
  return 0;
}

//custom function based on specific needs from match control
//used for C goal 
int shootThreeRemoveOne(){
  setIntakeSpeed(100);
  while(!getbottomLineInfo())
    wait(50, msec);
  rollerSpeed = 100;
  while(getbottomLineInfo())
    wait(50, msec);
  while(!getbottomLineInfo())
    wait(50, msec);
  while(getbottomLineInfo())
    wait(50, msec);
  wait(150, msec);
  setIntakeSpeed(-20);
  wait(600, msec);
  rollerSpeed = 0;
  setIntakeSpeed(0);
  return 0;
}

//custom function based on specific needs from match control
//used for F goal 
int shootOneRemoveTwo(){
  setIntakeSpeed(100);
  rollerSpeed = 100;
  while(!gettopLineInfo())
    wait(50, msec);
  while(gettopLineInfo())
    wait(50, msec);
  wait(100, msec);
  setIntakeSpeed(0);
  rollerSpeed = 0;
  return 0;
}

//custom function based on specific needs from match control
//used for I goal 
int shootTwoRemoveTwo(){
  rollerSpeed = 100;
  setIntakeSpeed(100);
  while(!getbottomLineInfo())
    wait(50, msec);
  while(getbottomLineInfo())
    wait(50, msec);
  while(!getbottomLineInfo())
    wait(50, msec);
  while(getbottomLineInfo())
    wait(50, msec);
  while(!getmiddleLineInfo())
    wait(50, msec);
  setIntakeSpeed(0);
  while(getmiddleLineInfo())
    wait(50, msec);
  wait(100, msec);
  rollerSpeed = 0;
  return 0;
}

//clean function to remove balls for autonomous
int acleanBalls() {
  setIntakeSpeed(-80);
  rollerSpeed = -100;
  wait(500, msec);
  rollerSpeed = 0;
  setIntakeSpeed(0);
  return 0;
}

//checks if rollers are running, otherwise starts them
void runRollersBack(){
  if(rollerSpeed < 25)
    rollerSpeed = 50;
}

//doet not continue until shoot is completed in autonRoller
void rollerHold(){
  while(!shootdone)
    wait(50, msec);
}

void resetRollers() {
  lRoller.resetRotation();
  rRoller.resetRotation();
}

// checks tempeatures of all intake motors are returns in a string which motors
// are hot
std::string tempInfoRollers() {
  std::string tempReturn;
  int loopCounter = 0;
  if (getLRollerTemp() > tempLimit)
    tempReturn = "R ";
  loopCounter++;
  if (getRRollerTemp() > tempLimit)
    tempReturn += "L ";
  loopCounter++;
  if (loopCounter == 0)
    tempReturn = "All Good";
  return tempReturn;
}