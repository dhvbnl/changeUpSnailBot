//standard c headers
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//vex specific headers
#include "v5.h"
#include "v5_vcs.h"
#include "robot-config.h"

//user defined headers
#include "preauton.h"
#include "opcontrol.h"
#include "drivetrain.h"
#include "controller.h"
#include "screen.h"
#include "intakes.h"
#include "rollers.h"
#include "mapping.h"
#include "odometry.h"

//global constant variables
const int tempLimit = 55;
const double voltageConverstion = 100/12;
const double odometryWheelToInch = 2.75/360;

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)