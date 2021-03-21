using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern motor lFront;
extern motor rollerBottom;
extern motor lBack;
extern signature Vision__REDBALL;
extern signature Vision__SIG_2;
extern signature Vision__SIG_3;
extern signature Vision__SIG_4;
extern signature Vision__SIG_5;
extern signature Vision__SIG_6;
extern signature Vision__SIG_7;
extern vision Vision;
extern motor rBack;
extern motor rIntake;
extern motor lIntake;
extern motor rollerTop;
extern motor rFront;
extern inertial Inertial;
extern line lineTop;
extern line lineBottom;
extern line lineMiddle;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );