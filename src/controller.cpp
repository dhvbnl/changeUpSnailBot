#include "vex.h"

const int deadzone = 1;

// joysticks getters

//returns position of respective axises taking in account a deadzone
int getAxis1Pos() 
{
  int axisPos = Controller.Axis1.position();
  axisPos = abs(axisPos) > deadzone ? axisPos : 0;
  return axisPos;

}
int getAxis2Pos() 
{
  int axisPos = Controller.Axis2.position();
  axisPos = abs(axisPos) > deadzone ? axisPos : 0;
  return axisPos;

}
int getAxis3Pos() 
{
  int axisPos = Controller.Axis3.position();
  axisPos = abs(axisPos) > deadzone ? axisPos : 0;
  return axisPos;

}
int getAxis4Pos() 
{
  int axisPos = Controller.Axis4.position();
  axisPos = abs(axisPos) > deadzone ? axisPos : 0;
  return axisPos;

}

// bumper getters
bool getL1Pos(){return Controller.ButtonL1.pressing();};
bool getL2Pos(){return Controller.ButtonL2.pressing();};
bool getR1Pos(){return Controller.ButtonR1.pressing();};
bool getR2Pos(){return Controller.ButtonR2.pressing();};

// button getters
bool getUpPos(){return Controller.ButtonUp.pressing();};
bool getRightPos(){return Controller.ButtonRight.pressing();};
bool getDownPos(){return Controller.ButtonDown.pressing();};
bool getLeftPos(){return Controller.ButtonLeft.pressing();};
bool getXPos(){return Controller.ButtonX.pressing();};
bool getAPos(){return Controller.ButtonA.pressing();};
bool getBPos(){return Controller.ButtonB.pressing();};
bool getYPos(){return Controller.ButtonY.pressing();};
