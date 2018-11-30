
#pragma once

#include <arduino.h>

namespace robot {
	namespace configuration {

		extern int32_t wheel_radius;
		extern int32_t robot_radius;
		extern bool radii_set;

		// sets wheel and robot radius
		void set_radii(int32_t wheel_radius, int32_t robot_radius);

	}
}
