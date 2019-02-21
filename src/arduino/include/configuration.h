
#pragma once

#include <Arduino.h>
#include "debug.h"

namespace robot {
	namespace configuration {

		extern int32_t wheel_radius;
		extern int32_t robot_radius;
		extern bool radii_set;

		// sets wheel and robot radius
		void set_wheel_radius(int32_t w_radius);
		void set_robot_radius(int32_t r_radius);
		void set_radii(int32_t wheel_radius, int32_t robot_radius);

	}
}
