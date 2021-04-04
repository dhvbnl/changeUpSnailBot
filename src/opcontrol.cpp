#include "vex.h"

void opcontrol()
{
  task driveMove(drivetrainControl);
  task intakeMove(intakeControl);
  task rollerMove(rollerControl);
  task screenChange(screenGUI);
  while(true)
  {
    wait(100, msec);
  }
}