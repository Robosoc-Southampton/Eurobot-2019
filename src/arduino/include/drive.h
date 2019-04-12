
/* drive.h
 *
 * This file contains code related to driving and MD25 control
 * - setup/loop functions used by `robot.*`
 */

#pragma once

#include <Arduino.h>
#include "debug.h"
#include "configuration.h"
#include "component/MD25.h"

namespace robot {
	namespace drive {
		extern MD25 *md25;
		extern bool is_moving, is_moving_forward;
		extern int32_t target_left_encoder_value, target_right_encoder_value;

		// move forward `distance`mm
		void forward(int16_t distance);

		// turn by `angle` degrees counterclockwise
		void turn(int16_t angle);

		// stop
		void stop();

		// sets the MD25 to use
		void set_md25(MD25 *md25);

		// update motor speeds for MD25 (internal)
		void update_motor_speeds();

		// take a positive distance delta and returns speed (0 -> 127) (internal)
		int16_t get_average_distance_travelled();

		// take a positive distance delta and returns speed (0 -> 127) (internal)
		uint8_t encoder_delta_to_speed(int32_t delta);

		// return the encoder value equivalent to a given distance (internal)
		int32_t distance_to_encoder_reading(int32_t distance);

		// return the encoder value equivalent to a given distance (internal)
		int16_t encoder_reading_to_distance(int32_t distance);
	}
}
