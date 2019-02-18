
#include "include/drive.h"

#define _3600_DIV_2PI 573l

namespace robot {
	namespace drive {

		int32_t target_left_encoder_value = 0, target_right_encoder_value = 0;
		MD25 *md25;
		bool is_moving = false;
		int16_t ALIGN_DISTANCE = -100; // 10cm
		uint8_t SPEED_THRESHOLD = 10u;
		int32_t ENCODER_DELTA_THRESHOLD = 10;

		void forward(int16_t distance) {
			int32_t encoder_value = distance_to_encoder_reading(distance);

			target_left_encoder_value  += encoder_value;
			target_right_encoder_value += encoder_value;
			is_moving = true;

			md25->resetEncoders();
			update_motor_speeds();
		}

		void turn(int16_t angle) {
			int16_t distance = angle * configuration::robot_radius * 10 / _3600_DIV_2PI;
			int32_t encoder_value = distance_to_encoder_reading(distance);

			target_left_encoder_value  -= encoder_value;
			target_right_encoder_value += encoder_value;
			is_moving = true;

			md25->resetEncoders();
			update_motor_speeds();
		}

		void reset() {
			is_moving = false;

			target_left_encoder_value = 0;
			target_right_encoder_value = 0;

			md25->stopMotors();
			md25->resetEncoders();
		}

		void stop() {
			is_moving = false;

			target_left_encoder_value  -= md25->readLeftEncoder();
			target_right_encoder_value -= md25->readRightEncoder();

			md25->resetEncoders();
			md25->stopMotors();
		}

		void set_md25(MD25 *_md25) {
			md25 = _md25;
		}

		void update_motor_speeds() {
			if (!is_moving) return;

			bool has_finished_movement = false;

			int32_t left_delta = target_left_encoder_value - md25->readLeftEncoder();
			int32_t right_delta = target_right_encoder_value - md25->readRightEncoder();

			uint8_t left_speed = 0, right_speed = 0;

			if (left_delta >= 0) {
				left_speed = 128u + encoder_delta_to_speed(left_delta);
			}
			else {
				left_speed = 128u - encoder_delta_to_speed(-left_delta);
			}

			if (right_delta >= 0) {
				right_speed = 128u + encoder_delta_to_speed(right_delta);
			}
			else {
				right_speed = 128u - encoder_delta_to_speed(-right_delta);
			}

			if (diff(left_speed, 128u) < SPEED_THRESHOLD && diff(right_speed, 128u) < SPEED_THRESHOLD) {
				if (abs(left_delta) < ENCODER_DELTA_THRESHOLD && abs(right_delta) < ENCODER_DELTA_THRESHOLD) {
					has_finished_movement = true;
				}
			}

			if (has_finished_movement) {
				stop();
				robot::send_message('s', 0); // send a status(0) message
			}
			else {
				md25->setLeftMotorSpeed(left_speed);
				md25->setRightMotorSpeed(right_speed);
			}
		}

		int16_t get_average_distance_travelled() {
			return encoder_reading_to_distance(( md25->readLeftEncoder()
				                               + md25->readRightEncoder() )
			                                   / 2);
		}

		uint8_t encoder_delta_to_speed(int32_t delta) {
			return delta > 30 ? 100 : 0;
		}

		int32_t distance_to_encoder_reading(int16_t distance) {
			return (int32_t) distance * _3600_DIV_2PI / robot::configuration::wheel_radius / 10l;
		}

		int16_t encoder_reading_to_distance(int32_t encoder_value) {
			return (int16_t) (encoder_value * robot::configuration::wheel_radius / _3600_DIV_2PI * 10);
		}

		uint8_t diff(uint8_t a, uint8_t b) {
			return a > b ? a - b : b - a;
		}

	}
}
