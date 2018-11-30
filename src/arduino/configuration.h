
#pragma once

#include <arduino.h>

namespace robot {
	namespace configuration {

		extern int16_t wheel_radius;
		extern int16_t robot_radius;
		extern bool radii_set;

		// sets wheel and robot radius
		void set_radii(int16_t wheel_radius, int16_t robot_radius);

	}
}
