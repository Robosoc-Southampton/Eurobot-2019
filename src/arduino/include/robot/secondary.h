
#pragma once

#define MESSAGE_BUFFER_SIZE 150

#include <Arduino.h>
#include <Stepper.h>
#include <Servo.h>
#include "../component/MD25.h"
#include "../component/SharpIR.h"
#include "../robot.h"
#include "../activities.h"
#include "../debug.h"
#include "../timeout.h"

#define ACTIVITY_LOWER_STEPPER 1
#define ACTIVITY_RAISE_STEPPER 2
#define ACTIVITY_LOWER_STEPPER_SMALL 3
#define ACTIVITY_RAISE_STEPPER_SMALL 4
#define ACTIVITY_BLINK_LED 5
#define ACTIVITY_CLOSE_GRIP 6
#define ACTIVITY_OPEN_GRIP 7
#define ACTIVITY_PULL_CORD 100

#define PULL_CORD_PIN 12

#define GRIP_SERVO_PIN 5

#define STEPPER_SPEED 400
#define STEPPER_ROTATIONS 200

#define LIFTING_STEPPER_1 8
#define LIFTING_STEPPER_2 9
#define LIFTING_STEPPER_3 10
#define LIFTING_STEPPER_4 11

uint16_t readComponentValue(uint16_t component_ID);

Activity* lookupActivity(uint16_t activity_ID);
