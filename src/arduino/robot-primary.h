
#pragma once

#include <arduino.h>
#include "MD25.h"
#include "robot.h"

uint16_t readComponentValue(uint16_t component_ID);

Activity* lookupActivity(uint16_t activity_ID);

void primary_setup();
void primary_loop();
