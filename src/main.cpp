#include "vex.h"

using namespace vex;
competition Competition;

int main() {
  Competition.autonomous(rightAlliance);
  Competition.drivercontrol(opcontrol);

  preauton();
  while (true) {
    wait(100, msec);
  }
}
