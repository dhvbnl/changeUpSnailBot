#include "vex.h"


void leftAlliance() {
  // 15 second program starting on the left side
}

void rightAlliance() {
  // 15 second program starting on the right side
}

void skills() {
  // 60 second program
  lFront.setStopping(brake);
  lBack.setStopping(brake);
  rBack.setStopping(brake);
  rFront.setStopping(brake);
  
  //thread pos(getPosition);
  //driveProfile(10, false);
    thread setspeed(rollerControl);
    //thread setintakes(intakeControl);
    //ahoardAllRollers();
    thread pos(getPosition);
    setPos(0, -30);

}
