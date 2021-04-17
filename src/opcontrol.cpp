#include "vex.h"

void opcontrol()
{
  //make sure drivetrain is set to coast
  setDrivetrainCreep();

  //start all control tasks
  task driveMove(drivetrainControl);
  task intakeMove(intakeControl);
  task rollerMove(rollerControl);
  task screenChange(screenGUI);
  
  while(true)
  {
    wait(100, msec);
  }
}