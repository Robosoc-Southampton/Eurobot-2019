
#pragma once

#include <arduino.h>

typedef void Sensor;

namespace robot {
	extern Sensor *distance_sensors;
	extern uint8_t distance_sensor_count;

	// sets the list of distance sensors
	void set_distance_sensors(uint8_t count, Sensor *sensors);

	// checks distance sensors
	void check_distance_sensors();
}
