#include "vex.h"

void preauton() {
  vexcodeInit();

  resetDrivetrain();
  resetIntakes();
  resetRollers();
  resetEncoders();

  setDrivetrainCreep();
  setIntakeCreep();
  setRollerLock();

  calibrateInertial();
  Controller.Screen.print(getLeftEncoderRotation());
}

