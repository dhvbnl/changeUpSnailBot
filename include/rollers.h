//constants

//constant for min value when sensor does not detect anything
const int lineMax = 68;

//complete functions
int rollerControl();

//setters
void setRollerSpeed();

void setRollerCreep();
void setRollerLock();

void setHoardStopTrue();
void setHoardStopFalse();
void setShootStartTrue();
void setShootStartFalse();
void setCleanStartTrue();
void setCleanStartFalse();

//getters
int getLRollerSpeed();
int getRRollerSpeed();
int getRollerSpeed();

int getLRollerTemp();
int getRRollerTemp();

bool gettopLineInfo();
bool getmiddleLineInfo();
bool getbottomLineInfo();

bool getDescoreState();
int getBallsDetected();


//control
void runMacros();
void stopAllMacros();
void descoreChooser();

int shootAllRollers();
int shootOneRollers();
int shootOneRollersSlow();
int cycleCombo();
int customMiddle();

int hoardAllRollers();
int resetForHoard();

int clearAllRollers();
int descoreOneBall();
int descoreTwoBalls();

void resetRollers();

//auton variations

int autonRoller(int hoardBalls, int descoreBalls);
int shoot2Corner();
int shoot1Corner();
int shoot2Side();
int shoot1Side();
int shootMiddleGoal();

int adescoreOneBall();
int adescoreTwoBalls();
int acleanBalls();

int leftHomeGoalCustom();
int leftHomeGoalCustomLess();
int middleGoalCustomIntake();
int sideGoalCustom();
int middleGoalCustom();
int rightHomeGoalCustom();

int shootOneRemoveTwo();
int shootTwoRemoveTwo();

void runRollersBack();
void rollerHold();
void waitDrive();

std::string tempInfoRollers();