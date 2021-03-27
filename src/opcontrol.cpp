#include "vex.h"
#include "drivetrain.h"
#include "controller.h"
#include "screen.h"
#include "intakes.h"

void opcontrol()
{
  vex::thread(drivetrainControl).detach();
  vex::thread(intakeControl).detach();
  vex::thread(screenGUI).detach();
  while(true)
  {
    wait(100, msec);
  }
}