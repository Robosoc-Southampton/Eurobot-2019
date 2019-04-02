
#include "include/drive.h"

namespace robot {
	namespace drive {

		int32_t target_left_encoder_value = 0, target_right_encoder_value = 0;
		MD25 *md25;
		bool is_moving = false;
		bool is_moving_forward = false;
		uint8_t SPEED_THRESHOLD = 5u;
		int32_t ENCODER_DELTA_THRESHOLD = 5;
		int32_t left_encoder_measurements[3] = {},
		       right_encoder_measurements[3] = {};

		void init_movement() {
			 left_encoder_measurements[0] =  left_encoder_measurements[1] = md25->readLeftEncoder();
			right_encoder_measurements[0] = right_encoder_measurements[1] = md25->readRightEncoder();

			is_moving = true;
			is_moving_forward = false;

			md25->setAcceleration(configuration::acceleration);
		}

		void forward(int16_t distance) {
			int32_t encoder_value = distance_to_encoder_reading(distance);

			target_left_encoder_value  += encoder_value;
			target_right_encoder_value += encoder_value;

			init_movement();

			rlogfd("Driving forward");
		}

		void turn(int16_t angle) {
			int32_t distance = 31415l * (int32_t) angle * (int32_t) configuration::robot_radius / 1800000l;
			int32_t encoder_value = distance_to_encoder_reading(distance);

			target_left_encoder_value  -= encoder_value;
			target_right_encoder_value += encoder_value;

			init_movement();

			rlogfd("Turning");
		}

		void stop() {
			rlogfd("Stopping");

			md25->stopMotors();

			is_moving = false;
			is_moving_forward = false;
			target_left_encoder_value  -= md25->readLeftEncoder();
			target_right_encoder_value -= md25->readRightEncoder();

			md25->resetEncoders();
		}

		void set_md25(MD25 *_md25) {
			md25 = _md25;
		}

		void update_motor_speeds() {
			if (!is_moving) return;

			uint8_t left_speed = 0, right_speed = 0;
			int32_t left_delta, right_delta;

			 left_encoder_measurements[2] =  left_encoder_measurements[1];
			right_encoder_measurements[2] = right_encoder_measurements[1];
			 left_encoder_measurements[1] =  left_encoder_measurements[0];
			right_encoder_measurements[1] = right_encoder_measurements[0];

			 left_encoder_measurements[0] = md25->readLeftEncoder();
			right_encoder_measurements[0] = md25->readRightEncoder();

			left_delta = target_left_encoder_value - *left_encoder_measurements;
			right_delta = target_right_encoder_value - *right_encoder_measurements;

			is_moving_forward = left_delta > 0 || right_delta > 0;

			left_speed = left_delta >= 0
				? 128u + encoder_delta_to_speed(left_delta)
				: 128u - encoder_delta_to_speed(-left_delta);
			right_speed = right_delta >= 0
				? 128u + encoder_delta_to_speed(right_delta)
				: 128u - encoder_delta_to_speed(-right_delta);


			if (left_speed - 128u < SPEED_THRESHOLD
			 && 128u - left_speed < SPEED_THRESHOLD
			 && right_speed - 128u < SPEED_THRESHOLD
			 && 128u - right_speed < SPEED_THRESHOLD
			 && abs(left_delta) < ENCODER_DELTA_THRESHOLD
			 && abs(right_delta) < ENCODER_DELTA_THRESHOLD
			 &&  left_encoder_measurements[0] ==  left_encoder_measurements[1]
			 && right_encoder_measurements[0] == right_encoder_measurements[1]
			 &&  left_encoder_measurements[1] ==  left_encoder_measurements[2]
			 && right_encoder_measurements[1] == right_encoder_measurements[2]
			) {
				rlogfd("Finished movement");
				stop();
				robot::send_message('s', 0); // send a status(0) message
			}
			else {
				md25->setLeftMotorSpeed(left_speed);
				md25->setRightMotorSpeed(right_speed);
			}
		}

		int16_t get_average_distance_travelled() {
			return encoder_reading_to_distance(( left_encoder_measurements[0]
				                               + right_encoder_measurements[0] )
			                                   / 2);
		}

		uint8_t encoder_delta_to_speed(int32_t delta) {
			return delta > robot::configuration::peak_speed ? robot::configuration::peak_speed : delta;
			// if (encoder_reading_to_distance(delta) > robot::configuration::peak_speed / robot::configuration::acceleration) {
			// 	return robot::configuration::peak_speed;
			// }

			// return encoder_reading_to_distance(delta);
		}

		int32_t distance_to_encoder_reading(int32_t distance) {
			return distance * 180000l / (int32_t) robot::configuration::wheel_radius / 3141l;
		}

		int16_t encoder_reading_to_distance(int32_t encoder_value) {
			return (int16_t) (encoder_value * (int32_t) robot::configuration::wheel_radius / 573l * 10l);
		}

	}
}
