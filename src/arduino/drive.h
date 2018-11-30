
#pragma once

#include <arduino.h>
#include "configuration.h"
#include "MD25.h"

namespace robot {
	namespace drive {
		extern int32_t target_left_encoder_value, target_right_encoder_value;
		extern MD25 *md25;
		extern bool is_moving;
		extern int16_t ALIGN_DISTANCE;

		// move forward `distance`mm
		void forward(int16_t distance);
		// turn by `angle` degrees counterclockwise
		void turn(int16_t angle);
		// stop
		void stop();

		void set_md25(MD25 *md25);

		// update motor speeds for MD25 stuff
		void update_motor_speeds();

		int32_t distance_to_encoder_reading(int16_t distance);
	}
}
