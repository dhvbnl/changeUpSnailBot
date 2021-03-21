#include "vex.h"

using namespace vex;
competition Competition;
void driveprogram();

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
}

void usercontrol(void) {
  driveprogram();
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
