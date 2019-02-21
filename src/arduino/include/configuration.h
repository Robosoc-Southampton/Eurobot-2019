
#pragma once

#include <Arduino.h>
#include "debug.h"

namespace robot {
	namespace configuration {

		extern int16_t wheel_radius
		             , robot_radius
		             , peak_speed
		             ;

		extern int16_t config_key;

		void set_config_key(int16_t key);
		void set_config_value(int16_t value);

	}
}
