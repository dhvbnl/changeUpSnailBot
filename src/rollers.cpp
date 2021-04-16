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
thread shootAll;
thread shootOne;
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

void setRollerSpeed(int rollerSpeed) {
  lRoller.spin(fwd, rollerSpeed, pct);
  rRoller.spin(fwd, rollerSpeed, pct);
}

void setRollerSpeedCustom(int customSpeed)
{
  rollerSpeed = customSpeed;
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
    shootOne = thread(shootOneRollers);
    descoreChooser();
  } else if (getL2Pos()) {
    stopAllMacros();
    hoardAll = thread(hoardAllRollers);
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

void stopAllMacros() {
  shootAll.interrupt();
  shootOne.interrupt();
  hoardAll.interrupt();
  descoreTwo.interrupt();
  descoreOne.interrupt();
}

void descoreChooser() {
  if(shootState == 2)
    descoreTwo = thread(descoreTwoBalls);
  else if(shootState == 1)
    descoreOne = thread(descoreOneBall);
}

void adescoreChooser() {
  if(shootState == 2)
    thread two(adescoreTwoBalls);
  else if(shootState == 1)
    thread one(adescoreOneBall);
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

int hoardAllRollers() {
  while (!getbottomLineInfo())
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

int clearAllRollers() {
  pauseIntake();
  setIntakeSpeed(-75);
  rollerSpeed = -100;
  wait(500, msec);
  rollerSpeed = 0;
  playIntake();
  return 0;
}


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
      rollerSpeed = 25;
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
  wait(150, msec);
  rollerSpeed = 0;
  while(!descoreState)
    wait(50, msec);
  shootdone = true;
  rollerSpeed = 0;
  setIntakeSpeed(-20);
  wait(400, msec);
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

int shoot2Corner(){
  autonRoller(2, 2);
  return 0;
}

int shoot1Corner(){
  autonRoller(1, 2);
  return 0;
}

int shoot2Side(){
  autonRoller(2, 1);
  return 0;
}

int shoot1Side(){
  autonRoller(1, 1);
  return 0;
}

int adescoreOneBall() {
  descoreState = false;
    setIntakeSpeed(100);
    while (getbottomLineInfo())
      wait(50, msec);
    while(!getbottomLineInfo())
      wait(50, msec);
    setIntakeSpeed(0);
  descoreState = true;
  return 0;
}

int adescoreTwoBalls() {
  descoreState = false;
    setIntakeSpeed(100);
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
    rollerSpeed = 0;
    while(!getbottomLineInfo())
      wait(50, msec);
    setIntakeSpeed(0);
  descoreState = true;
  return 0;
}

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

void runRollersBack(){
  if(rollerSpeed < 25)
    rollerSpeed = 50;
}

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