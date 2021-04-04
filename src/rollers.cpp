#include "rollers.h"
#include "intakes.h"
#include "controller.h"

//global rollerSpeed checker
int rollerSpeed = 0;
//global descore checker
int shootState = 2;
//time for shoot exepction handling
timer alignRollers = timer();


//macro threads;
thread hoardAll;
thread shootAll;
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

// getters

int getLRollerSpeed() { return lRoller.velocity(pct); };
int getRRollerSpeed() { return rRoller.velocity(pct); };

int getLRollerTemp() { return lRoller.temperature(celsius); };
int getRRollerTemp() { return rRoller.temperature(celsius); };

bool gettopLineInfo() { return lineTop.value(pct) < lineMax; };
bool getmiddleLineInfo() { return lineMiddle.value(pct) < lineMax; };
bool getbottomLineInfo() { return lineBottom.value(pct) < lineMax; };

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

int hoardAllRollers() {
  while (!getbottomLineInfo())
    wait(50, msec);
  rollerSpeed = 30;
  while (!getmiddleLineInfo())
    wait(50, msec);
  rollerSpeed = 0;
  while (!getbottomLineInfo())
    wait(50, msec);
  rollerSpeed = 20;
  while (!gettopLineInfo())
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

void resetRollers() {
  lRoller.resetRotation();
  rRoller.resetRotation();
}

// checks tempeatures of all intake motors are returns in a string which motors
// are hot
std::string tempInfoRollers() {
  std::string tempReturn;
  int loopCounter = 0;
  if (getLRollerTemp() > tempLimitRollers)
    tempReturn = "R ";
  loopCounter++;
  if (getRRollerTemp() > tempLimitRollers)
    tempReturn += "L ";
  loopCounter++;
  if (loopCounter == 0)
    tempReturn = "All Good";
  return tempReturn;
}
