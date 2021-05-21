#include "vex.h"

using namespace vex;
competition Competition;

int main() {
  Competition.autonomous(rightAlliance);
  Competition.drivercontrol(opcontrol);
  
  Competition.bStopAllTasksBetweenModes = true;

  preauton();
  while (true) {
    wait(100, msec);
  }
}
