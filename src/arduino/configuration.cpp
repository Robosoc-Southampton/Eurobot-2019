
#include "include/configuration.h"

namespace robot {
	namespace configuration {

		int32_t wheel_radius = 10;
		int32_t robot_radius = 10;

		bool radii_set = false;

		void set_wheel_radius(int32_t w_radius) {
			wheel_radius = w_radius;
			rlog("Wheel radius was set");
		}

		void set_robot_radius(int32_t r_radius) {
			robot_radius = r_radius;
			rlog("Robot radius was set");
		}

		void set_radii(int32_t w_radius, int32_t r_radius) {
			wheel_radius = w_radius;
			robot_radius = r_radius;
			radii_set = true;
		}

	}
}
