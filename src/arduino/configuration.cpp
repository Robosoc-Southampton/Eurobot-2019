
#include "configuration.h"

namespace robot {
	namespace configuration {

		int16_t wheel_radius = 0;
		int16_t robot_radius = 0;

		bool radii_set = false;

		void set_radii(int16_t w_radius, int16_t r_radius) {
			wheel_radius = w_radius;
			robot_radius = r_radius;
			radii_set = true;
		}

	}
}
