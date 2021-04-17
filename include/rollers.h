//constants
const int lineMax = 68;

//complete functions
int rollerControl();

//setters
void setRollerSpeed();
void setRollerSpeed(int rollerSpeed);
void setRollerSpeedCustom(int customSpeed);

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
int shootTwoRollers();

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
int shootThreeRemoveOne();
int shootOneRemoveTwo();
int shootTwoRemoveTwo();

void runRollersBack();
void rollerHold();

std::string tempInfoRollers();
