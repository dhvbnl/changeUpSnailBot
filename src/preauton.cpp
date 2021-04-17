#include "vex.h"

void preauton() {
  vexcodeInit();

  //reset all sensors
  resetDrivetrain();
  resetIntakes();
  resetRollers();
  resetEncoders();

  //set motor stopping type
  setDrivetrainCreep();
  setIntakeCreep();
  setRollerLock();

  //calibrate inertial sensor
  calibrateInertial();
  Controller.Screen.print("Preauton Complete");
}

