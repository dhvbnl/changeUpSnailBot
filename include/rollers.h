#include "vex.h"

//constants
const int lineMax = 68;
const int tempLimitRollers = 55;


//complete functions
int rollerControl();

//setters
void setRollerSpeed();

void setRollerCreep();
void setRollerLock();

//getters
int getLRollerSpeed();
int getRRollerSpeed();

int getLRollerTemp();
int getRRollerTemp();

bool gettopLineInfo();
bool getmiddleLineInfo();
bool getbottomLineInfo();
int getBallsDetected();

//control
void runMacros();
void stopAllMacros();
void descoreChooser();

int shootAllRollers();
int hoardAllRollers();
int clearAllRollers();
int descoreOneBall();
int descoreTwoBalls();

void resetRollers();

std::string tempInfoRollers();
