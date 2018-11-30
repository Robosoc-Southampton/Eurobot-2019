
#pragma once

#include <arduino.h>

namespace robot {
	namespace drive {
		extern int32_t target_left_encoder_value, target_right_encoder_value;
		extern bool is_moving;

		void update_motor_speeds();
	}
}
