
#pragma once

#include <Arduino.h>
#include <Stepper.h>
#include <Servo.h>
#include "../component/MD25.h"
#include "../component/SharpIR.h"
#include "../robot.h"
#include "../activities.h"
#include "../debug.h"
#include "../timeout.h"

uint16_t readComponentValue(uint16_t component_ID);

Activity* lookupActivity(uint16_t activity_ID);
