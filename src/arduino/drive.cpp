
#include "drive.h"

#define _3600_DIV_2PI 573

namespace robot {
	namespace drive {

		int32_t target_left_encoder_value = 0, target_right_encoder_value = 0;
		MD25 *md25;
		bool is_moving = false;
		int16_t ALIGN_DISTANCE = -100; // 10cm

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
			// TODO

			bool has_finished_movement = false;

			if (has_finished_movement) {
				stop();
				robot::send_message('s', 0); // send a status(0) message
			}
		}

		int16_t get_average_distance_travelled() {
			return encoder_reading_to_distance(( md25->readLeftEncoder()
				                               + md25->readRightEncoder() )
			                                   / 2);
		}

		int32_t distance_to_encoder_reading(int16_t distance) {
			return distance * _3600_DIV_2PI / robot::configuration::wheel_radius / 10;
		}

		int16_t encoder_reading_to_distance(int32_t encoder_value) {
			return (int16_t) (encoder_value * robot::configuration::wheel_radius / _3600_DIV_2PI * 10);
		}

	}
}
