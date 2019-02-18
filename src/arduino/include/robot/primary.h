
#pragma once

#include <Arduino.h>
#include "../component/MD25.h"
#include "../robot.h"
#include "../activities.h"
#include "../debug.h"

uint16_t readComponentValue(uint16_t component_ID);

Activity* lookupActivity(uint16_t activity_ID);
