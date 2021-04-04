#include "vex.h"
#include "opcontrol.h"

using namespace vex;
competition Competition;

void preauton();

void autonomous(void) 
{
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(opcontrol);

  preauton();
  while (true) {
    wait(100, msec);
  }
}
