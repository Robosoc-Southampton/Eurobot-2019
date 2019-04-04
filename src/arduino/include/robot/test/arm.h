
/* Arm test
 *
 * Tests the robotic arm
 */

#pragma once

#include <Arduino.h>
#include <Servo.h>
#include <Stepper.h>
#include "../../messages.h"

// const uint8_t ARM_STEPPER_1 = 22;
// const uint8_t ARM_STEPPER_2 = 24;
// const uint8_t ARM_STEPPER_3 = 26;
// const uint8_t ARM_STEPPER_4 = 28;
// const uint8_t SERVO_PRIMARY = 2;
// const uint8_t SERVO_SECONDARY = 3;
// const uint8_t SERVO_GRABBER = 4;

const uint8_t ARM_STEPPER_1 = 22;
const uint8_t ARM_STEPPER_2 = 24;
const uint8_t ARM_STEPPER_3 = 26;
const uint8_t ARM_STEPPER_4 = 28;
const uint8_t SERVO_PRIMARY = 4;
const uint8_t SERVO_SECONDARY = 3;
const uint8_t SERVO_GRABBER = 2;

// primary:
//  160 closed (^)
//  30 openish (|)

// secondary
// 65 extended
// 30 angled

// grabber:
// 170 open wide
// 60 open
// 15 closed
