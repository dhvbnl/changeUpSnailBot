#include "preauton.h"

void preauton(void) {
  vexcodeInit();

  lFront.setBrake(coast);
  lBack.setBrake(coast);
  rBack.setBrake(coast);
  rFront.setBrake(coast);

  rRoller.setBrake(brake);
  lRoller.setBrake(brake);
}

