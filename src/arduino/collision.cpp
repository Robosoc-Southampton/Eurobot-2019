
#include "collision.h"

namespace robot {

	Sensor *distance_sensors = nullptr;
	uint8_t distance_sensor_count = 0;

	void set_distance_sensors(uint8_t count, Sensor *sensors) {
		distance_sensors = sensors;
		distance_sensor_count = count;
	}

	void check_distance_sensors() {
		// check distance sensors in parallel
		for (Sensor *sensor = distance_sensors; sensor != distance_sensors + distance_sensor_count; ++sensor) {
			// ...
		}
	}

}
