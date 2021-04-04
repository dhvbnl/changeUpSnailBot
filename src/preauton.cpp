#include "vex.h"

void preauton() {
  vexcodeInit();

  resetDrivetrain();
  resetIntakes();
  resetRollers();

  setDrivetrainCreep();
  setIntakeCreep();
  setRollerLock();

  calibrateInertial();

}

