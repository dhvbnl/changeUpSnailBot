#include "preauton.h"
#include "drivetrain.h"
#include "intakes.h"
#include "rollers.h"

void preauton(void) {
  vexcodeInit();

  resetDrivetrain();
  resetIntakes();
  resetRollers();

  setDrivetrainCreep();
  setIntakeCreep();
  setRollerLock();

  calibrateInertial();

}

