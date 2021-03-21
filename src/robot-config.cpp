#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor lFront = motor(PORT11, ratio18_1, true);
motor rollerBottom = motor(PORT8, ratio18_1, false);
motor lBack = motor(PORT12, ratio18_1, true);
/*vex-vision-config:begin*/
signature Vision__REDBALL = signature (1, 8813, 11205, 10009, -1503, -105, -804, 3, 0);
vision Vision = vision (PORT2, 50, Vision__REDBALL);
/*vex-vision-config:end*/
motor rBack = motor(PORT19, ratio18_1, false);
motor rIntake = motor(PORT16, ratio18_1, true);
motor lIntake = motor(PORT15, ratio18_1, false);
motor rollerTop = motor(PORT10, ratio18_1, false);
motor rFront = motor(PORT18, ratio18_1, false);
inertial Inertial = inertial(PORT3);
line lineTop = line(Brain.ThreeWirePort.A);
line lineBottom = line(Brain.ThreeWirePort.C);
line lineMiddle = line(Brain.ThreeWirePort.B);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}