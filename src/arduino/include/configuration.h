
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

		// sets the key to be written to
		void set_config_key(int16_t key);

		// sets the configuration value for the currently selected key
		void set_config_value(int16_t value);

	}
}
