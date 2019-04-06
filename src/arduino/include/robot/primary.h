
#pragma once

#define MESSAGE_BUFFER_SIZE 250

#include <Arduino.h>
#include <Stepper.h>
#include <Servo.h>
#include "../component/MD25.h"
#include "../component/SharpIR.h"
#include "../robot.h"
#include "../activities.h"
#include "../debug.h"
#include "../timeout.h"

#define ACTIVITY_ROTATE_CAROUSEL 1
#define ACTIVITY_LOWER_ARM 3
#define ACTIVITY_RAISE_ARM 4
#define ACTIVITY_BLINK_LED 5
#define ACTIVITY_LOWER_ARM_FROM_TOP_TO_SIDE 10
#define ACTIVITY_RAISE_ARM_FROM_SIDE_TO_TOP_RED 11
#define ACTIVITY_RAISE_ARM_FROM_SIDE_TO_TOP_GREEN 12
#define ACTIVITY_RAISE_ARM_FROM_SIDE_TO_TOP_BLUE 13
#define ACTIVITY_LOWER_ARM_INTO_CAROUSEL 14
#define ACTIVITY_RAISE_ARM_FROM_CAROUSEL 15
#define ACTIVITY_PULL_CORD 100

#define PULL_CORD_PIN 12

#define PRIMARY_SERVO_PIN 4
#define SECONDARY_SERVO_PIN 3
#define GRABBER_SERVO_PIN 2

#define STEPPER_SPEED 50
#define STEPPER_ROTATIONS 200

#define ARM_STEPPER_1 22
#define ARM_STEPPER_2 24
#define ARM_STEPPER_3 26
#define ARM_STEPPER_4 28

#define CAROUSEL_STEPPER_1 8
#define CAROUSEL_STEPPER_2 9
#define CAROUSEL_STEPPER_3 10
#define CAROUSEL_STEPPER_4 11

#define ARM_GRABBER_CLOSED 15
#define ARM_GRABBER_OPEN 50

uint16_t readComponentValue(uint16_t component_ID);

Activity* lookupActivity(uint16_t activity_ID);
