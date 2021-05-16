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

double getInertialRotation();
double getInertialHeading();

double getLeftEncoderRotation();
double getRightEncoderRotation();

int getLFrontTemp();
int getLBackTemp();
int getRBackTemp();
int getRFrontTemp();

//control 
void resetDrivetrain();
void resetEncoders();
void calibrateInertial();
void drivetrainTurn(double targetdeg);
void timeDrive(double speed, int timeLength);
void arcturn (double left, double right, double turnangle);

std::string tempInfoDrive();


