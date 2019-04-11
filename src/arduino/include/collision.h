
/* collision.h
 *
 * This file contains code related to distance sensing/collisions
 * - setup functions used in entry point setup
 * - setup/loop functions used by `robot.*`
 */

#pragma once

#include <Arduino.h>
#include "debug.h"
#include "component/UltraSonic.h"

typedef UltraSonic DistanceSensor;

namespace robot {
	extern DistanceSensor *distance_sensors;
	extern uint8_t distance_sensor_count;
	extern bool distance_sensor_enabled_mask;

	// sets the list of distance sensors
	// should be called prior to robot::setup()
	void set_distance_sensors(uint8_t count, DistanceSensor *sensors);

	// checks distance sensors, returns true if a collision is detected
	bool check_distance_sensors();

	// reads the value of distance sensor at index `sensor`
	int16_t read_distance_sensor(uint8_t sensor);

	// enables and disables all distance sensors
	void enable_distance_sensors();
	void disable_distance_sensors();
	// enables and disables specific distance sensors
	void enable_distance_sensor(uint8_t sensor);
	void disable_distance_sensor(uint8_t sensor);
}
