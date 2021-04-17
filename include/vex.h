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
#include "motion-profile.h"

//global constant variables

//distance between tracking wheels
const double trackWidth = 4.625;

//maximum safe temperature for motors
const int tempLimit = 55;

//convert between velocity and voltage.
const double voltageConverstion = 100/12;

const double convertInches = (2.75 * M_PI) / 360;
const double convertDegrees = 360 / (2.75 * M_PI);

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)