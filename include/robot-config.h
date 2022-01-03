using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller;
extern motor lFront;
extern motor lBack;
extern motor rBack;
extern motor rFront;

extern motor rIntake;
extern motor lIntake;

extern motor lRoller;
extern motor rRoller;

extern inertial Inertial;

extern line lineTop;
extern line lineMiddle;
extern line lineBottom;

extern encoder encoderLeft;
extern encoder encoderRight;
extern encoder encoderH;

void  vexcodeInit();