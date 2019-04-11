
#include "include/collision.h"

namespace robot {

	DistanceSensor *distance_sensors = nullptr;
	uint8_t distance_sensor_count = 0;
	bool distance_sensor_enabled_mask = true;

	void set_distance_sensors(uint8_t count, DistanceSensor *sensors) {
		distance_sensors = sensors;
		distance_sensor_count = count;
	}

	bool check_distance_sensors() {
		bool is_collision_detected = false;

		if (!distance_sensor_enabled_mask) return false;

		for (DistanceSensor *sensor = distance_sensors; sensor != distance_sensors + distance_sensor_count; ++sensor) {
			if (sensor->enabled && sensor->read() > sensor->trigger_distance) {
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
