//constants
const double quadraticA = 0.12;
const int powerRatioB = 1;

//complete functions
int drivetrainControl();

//setters
void setDrivetrainSpeed(int leftSpeed, int rightSpeed);

void setDrivetrainCreep();
void setDrivetrainLock();

//getters
int getLeftSpeedInLinear();
int getRightSpeedInLinear();

int getLeftSpeedInSlew();
int getRightSpeedInSlew();

int getLFrontSpeed();
int getLBackSpeed();
int getRBackSpeed();
int getRFrontSpeed();

int getLeftEncoderRotation();
int getRightEncoderRotation();

int getLFrontTemp();
int getLBackTemp();
int getRBackTemp();
int getRFrontTemp();

//control 
void resetDrivetrain();
void resetEncoders();
void calibrateInertial();

std::string tempInfoDrive();


