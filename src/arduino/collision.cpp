
#include "include/collision.h"

namespace robot {

	SharpIR *distance_sensors = nullptr;
	uint8_t distance_sensor_count = 0;

	void set_distance_sensors(uint8_t count, SharpIR *sensors) {
		distance_sensors = sensors;
		distance_sensor_count = count;
	}

	bool check_distance_sensors() {
		bool is_collision_detected = false;
		int16_t THRESHOLD = 0;

		for (SharpIR *sensor = distance_sensors; sensor != distance_sensors + distance_sensor_count; ++sensor) {
			if (sensor->enabled && sensor->read() < THRESHOLD) {
				return true;
			}
		}

		return false;
	}

	int16_t read_distance_sensor(uint8_t sensor) {
		return distance_sensors[sensor].read();
	}

	void enable_distance_sensors() {
		for (uint8_t sensor = 0; sensor < distance_sensor_count; ++sensor)
			distance_sensors[sensor].enabled = true;
	}

	void disable_distance_sensors() {
		for (uint8_t sensor = 0; sensor < distance_sensor_count; ++sensor)
			distance_sensors[sensor].enabled = false;
	}

	void enable_distance_sensor(uint8_t sensor) {
		distance_sensors[sensor].enabled = true;
	}

	void disable_distance_sensor(uint8_t sensor) {
		distance_sensors[sensor].enabled = false;
	}

}
