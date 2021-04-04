#include "vex.h"

using namespace vex;
competition Competition;

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
