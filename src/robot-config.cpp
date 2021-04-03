#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller = controller(primary);


motor lFront = motor(PORT3, ratio6_1, false);
motor lBack = motor(PORT2, ratio6_1, false);

motor rBack = motor(PORT9, ratio6_1, true);
motor rFront = motor(PORT10, ratio6_1, true);


motor rIntake = motor(PORT20, ratio6_1, true);
motor lIntake = motor(PORT13, ratio6_1, false);

motor lRoller = motor(PORT12, ratio6_1, true);
motor rRoller = motor(PORT19, ratio6_1, false);


inertial Inertial = inertial(PORT17);

line lineTop = line(Brain.ThreeWirePort.B);
line lineMiddle = line(Brain.ThreeWirePort.C);
line lineBottom = line(Brain.ThreeWirePort.D);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

void vexcodeInit( void ) {
  // nothing to initialize
}