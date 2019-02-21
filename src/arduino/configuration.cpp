
#include "include/configuration.h"

namespace robot {
	namespace configuration {

		int16_t wheel_radius = 10
		      , robot_radius = 10
		      , peak_speed = 10
		      ;

		int16_t config_key = 0;

		void set_config_key(int16_t key) {
			config_key = key;
		}

		void set_config_value(int16_t value) {
			switch (config_key) {
				case 1: robot_radius = value; break;
				case 2: wheel_radius = value; break;
				case 3: peak_speed = value; break;
			}
		}

	}
}
