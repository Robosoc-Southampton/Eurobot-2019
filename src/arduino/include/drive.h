
#pragma once

#include <Arduino.h>
#include "configuration.h"
#include "robot.h"
#include "component/MD25.h"

namespace robot {
	namespace drive {
		extern int32_t target_left_encoder_value, target_right_encoder_value;
		extern MD25 *md25;
		extern bool is_moving;
		extern int16_t ALIGN_DISTANCE;
		extern uint8_t SPEED_THRESHOLD;
		extern int32_t ENCODER_DELTA_THRESHOLD;

		// move forward `distance`mm
		void forward(int16_t distance);
		// turn by `angle` degrees counterclockwise
		void turn(int16_t angle);
		// reset target encoder values and md25 encoder readings
		void reset();
		// stop
		void stop();

		void set_md25(MD25 *md25);

		// update motor speeds for MD25 stuff
		void update_motor_speeds();

		// return the mean average of the distance travelled by both motors
		int16_t get_average_distance_travelled();

		// take a positive distance delta and returns speed (0 -> 127)
		uint8_t encoder_delta_to_speed(int32_t delta);

		// return the encoder value equivalent to a given distance
		int32_t distance_to_encoder_reading(int16_t distance);

		// return the distance equivalent to a given encoder value
		int16_t encoder_reading_to_distance(int32_t encoder_value);

		// return the absolute difference between two values
		uint8_t diff(uint8_t a, uint8_t b);
	}
}
