//constants
const int lineMax = 68;

//complete functions
int rollerControl();

//setters
void setRollerSpeed();
void setRollerSpeed(int rollerSpeed);

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

//auton variations
int ahoardAllRollers();
int adescoreOneBall();
int adescoreTwoBalls();
int acleanBalls();
void adescoreChooser();
void intake(int speed);

std::string tempInfoRollers();
