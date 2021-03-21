#include "drivefunctions.h"
#include "vex.h"

vex::task taskHOLD;
vex::task taskSHOOT;
vex::task taskREMOVETWO;
vex::task taskREMOVEONE;
vex::task taskCLEAN;

void driveprogram(void) {
  lFront.setBrake(coast);
  rFront.setBrake(coast);
  lBack.setBrake(coast);
  rBack.setBrake(coast);
  taskSHOOT = vex::task(shootCustom);

  while (1) {
    // Drivetrain
    leftdrivespeed =
        Controller1.Axis3.position() + Controller1.Axis4.position();
    rightdrivespeed =
        Controller1.Axis3.position() - Controller1.Axis4.position();

    lFront.spin(directionType::fwd, leftdrivespeed, pct);
    lBack.spin(directionType::fwd, leftdrivespeed, pct);
    rBack.spin(directionType::fwd, rightdrivespeed, pct);
    rFront.spin(directionType::fwd, rightdrivespeed, pct);

    // Intake
    if (intakestate == true) {
      intakespeed = Controller1.Axis2.position();
    }

    lIntake.spin(directionType::fwd, intakespeed, pct);
    rIntake.spin(directionType::fwd, intakespeed, pct);

    // Rollers
    Controller1.ButtonR2.pressed(togglerollerOFF);

    if (Controller1.ButtonA.pressing()) {
      taskHOLD = vex::task(ballHoard);
    }
    
    if (Controller1.ButtonX.pressing()) {
      //Controller1.Screen.print("HIAUSFH");
      taskREMOVETWO = vex::task(removeTwo);
    }
    if (Controller1.ButtonB.pressing()) {
      taskCLEAN = vex::task(ballClean);
    } 

    rollerTop.spin(forward, rollertopspeed, pct);
    rollerBottom.spin(forward, rollerbottomspeed, pct);
    wait(10, msec);
  }
}

// roller control macros
int ballHoard() {
  while (lineTop.value(pct) >= 65) {
    rollertopspeed = 0;
    rollerbottomspeed = 100;
    if (Controller1.ButtonL1.pressing() || Controller1.ButtonR2.pressing()) {
      return 0;
    }
  }
  while (lineMiddle.value(pct) >= 65) {
    rollertopspeed = 0;
    rollerbottomspeed = 40;
    if (Controller1.ButtonL1.pressing() || Controller1.ButtonR2.pressing()) {
      return 0;
    }
  }
  rollertopspeed = 0;
  rollerbottomspeed = 0;
  return 0;
}

int removeOne() {
  while (lineBottom.value(pct) >= 65) {
    rollertopspeed = 0;
    rollerbottomspeed = 0;
    if (Controller1.ButtonL1.pressing() || Controller1.ButtonR2.pressing()) {
      return 0;
    }
  }
  rollertopspeed = 0;
  rollerbottomspeed = 0;
  return 0;
}
int removeTwo() {
  while (lineMiddle.value(pct) >= 65) {
    rollertopspeed = 0;
    rollerbottomspeed = 50;
    if (Controller1.ButtonA.pressing() || Controller1.ButtonR2.pressing()) {
      return 0;
    }
  }
  while (lineBottom.value(pct) >= 65) {
    rollertopspeed = 0;
    rollerbottomspeed = 0;
    if (Controller1.ButtonA.pressing() || Controller1.ButtonR2.pressing() ) {
      return 0;
    }
  }
  goalType = !goalType;
  rollertopspeed = 0;
  rollerbottomspeed = 0;
  return 0;
}
int ballClean() {
  rollertopcontrol = -100;
  rollerbottomcontrol = -100;
  intakestate = false;
  if (lineTop.value(pct) < 65) {
    rollertopspeed = rollertopcontrol;
    rollerbottomspeed = rollerbottomcontrol;
    intakespeed = -100;
    vex::task::sleep(700);
    intakestate = true;
    rollertopspeed = 0;
    rollerbottomspeed = 0;
  } else if (lineMiddle.value(pct) < 65) {
    rollertopspeed = rollertopcontrol;
    rollerbottomspeed = rollerbottomcontrol;
    intakespeed = -100;
    vex::task::sleep(600);
    intakestate = true;
    rollertopspeed = 0;
    rollerbottomspeed = 0;
  } else {
    rollertopspeed = rollertopcontrol;
    rollerbottomspeed = rollerbottomcontrol;
    intakespeed = -100;
    vex::task::sleep(400);
    intakestate = true;
    rollertopspeed = 0;
    rollerbottomspeed = 0;
  }
  return 0;
}
int ballShoot() {
  rollertopcontrol = 100;
  rollerbottomcontrol = 100;
  intakestate = false;
  if (lineBottom.value(pct) < 65) {
    intakespeed = 100;
    rollertopspeed = rollertopcontrol;
    rollerbottomspeed = rollerbottomcontrol;
    vex::task::sleep(200);
    rollertopspeed = 80;
    vex::task::sleep(600);
    rollertopspeed = 0;
  } else if (lineMiddle.value(pct) < 65) {
    intakespeed = 100;
    rollertopspeed = rollertopcontrol;
    rollerbottomspeed = rollerbottomcontrol;
    vex::task::sleep(200);
    intakespeed = 80;
    rollerbottomspeed = 0;
    vex::task::sleep(600);
    rollertopspeed = 0;
   // ballHoardMod();
  } else {
    intakespeed = 100;
    rollertopspeed = rollertopcontrol;
    rollerbottomspeed = rollerbottomcontrol;
    vex::task::sleep(300);
    intakespeed = 70;
    rollertopspeed = 0;
    //ballHoardMod();
  }
  rollerbottomspeed = 0;
  intakespeed = -40;
  vex::task::sleep(700);
  intakestate = true;
  ballClean();
  return 0;
}

int shootCustom() {
  while (true) {
    if (Controller1.ButtonR1.pressing()) {
      rollertopcontrol = 100;
      rollerbottomcontrol = 100;
      rollertopspeed = rollertopcontrol;
      rollerbottomspeed = rollerbottomcontrol;
      vex::task::sleep(1000);
      rollertopspeed = 0;
      rollerbottomspeed = 0;
    } else if (Controller1.ButtonL1.pressing()) {
      rollertopcontrol = 100;
      rollerbottomcontrol = 100;
      rollertopspeed = rollertopcontrol;
      rollerbottomspeed = rollerbottomcontrol;
      vex::task::sleep(200);
      rollerbottomspeed = 0;
      vex::task::sleep(300);
      rollertopspeed = 0;
    } else if (Controller1.ButtonL2.pressing()) {
      rollertopcontrol = 100;
      rollerbottomcontrol = 100;
      rollertopspeed = rollertopcontrol;
      rollerbottomspeed = rollerbottomcontrol;
      vex::task::sleep(600);
      rollertopspeed = 0;
      rollerbottomspeed = 0;
    }
  }
  return 0;
}

void togglerollerOFF() {
  vex::task::stop(ballHoard);
  vex::task::stop(ballShoot);
  rollertopspeed = 0;
  rollerbottomspeed = 0;
  intakestate = true;
}
