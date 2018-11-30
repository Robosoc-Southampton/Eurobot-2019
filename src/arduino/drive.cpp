
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

			md25->resetEncoders();
			md25->stopMotors();
		}

		void set_md25(MD25 *_md25) {
			md25 = _md25;
		}

		void update_motor_speeds() {
			// TODO
		}

		int32_t distance_to_encoder_reading(int16_t distance) {
			return distance * _3600_DIV_2PI / robot::configuration::wheel_radius / 10;
		}

	}
}
