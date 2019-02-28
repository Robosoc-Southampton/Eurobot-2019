
#pragma once

#include <Arduino.h>
#include "robot.h"
#include "drive.h"
#include "component/SharpIR.h"

namespace robot {
	extern SharpIR *distance_sensors;
	extern uint8_t distance_sensor_count;

	// sets the list of distance sensors
	void set_distance_sensors(uint8_t count, SharpIR *sensors);

	// checks distance sensors, returns true if a collision is detected
	bool check_distance_sensors();

	// reads the value of distance sensor at index `sensor`
	int16_t read_distance_sensor(uint8_t sensor);

	// enables and disables all distance sensors
	void enable_distance_sensors();
	void disable_distance_sensors();
	// enables and distables specific distance sensors
	void enable_distance_sensor(uint8_t sensor);
	void disable_distance_sensor(uint8_t sensor);
}
