
#include "include/collision.h"

int t2_counter;

namespace robot {

	DistanceSensor *distance_sensors = nullptr;
	uint8_t distance_sensor_count = 0;
	bool distance_sensor_enabled_mask = true;

	void setup_collision() {
		// noInterrupts();
		// t2_counter = 21;
		// TCCR2A = 0;
		// TCCR2B = 0;
		// // TCCR2B |= _BV(CS20) | _BV(CS21) | _BV(CS22);
		// TCCR2B |= _BV(CS21);
		// // TCCR2B |= _BV(CS20);
		// TCNT2 = t2_counter;
		// TIMSK2 |= _BV(TOIE2);
		// interrupts();
	}

	void set_distance_sensors(uint8_t count, DistanceSensor *sensors) {
		distance_sensors = sensors;
		distance_sensor_count = count;
	}

	int check_distance_sensors() {
		if (!distance_sensor_enabled_mask) return -1;

		for (int i = 0; i < distance_sensor_count; ++i) {
			DistanceSensor *sensor = distance_sensors + i;

			if (sensor->enabled && sensor->measure_distance(UltraSonic::distance_to_time(sensor->trigger_distance) * 2) < sensor->trigger_distance) {
				return i;
			}
		}

		return -1;
	}

	int16_t read_distance_sensor(uint8_t sensor) {
		return distance_sensors[sensor].measure_distance(1000000);
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

// ISR(TIMER2_OVF_vect){
// 	TCNT2 = t2_counter;

// 	for (int i = 0; i < robot::distance_sensor_count; ++i) {
// 		robot::distance_sensors[i].checkEcho();
// 	}
// }
