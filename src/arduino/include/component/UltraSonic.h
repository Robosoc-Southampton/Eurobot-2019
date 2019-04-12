
#pragma once

#include <Arduino.h>
#include <limits.h>
#include "../debug.h"

// the minimum time in microseconds between pulses
#define ULTRASONIC_MIN_DELAY 50000

class UltraSonic {
public:
	int16_t trigger_distance;
	bool enabled = true;

	// creates the UltraSonic object, with the digital pins it's connected to
	UltraSonic(uint8_t trigger_pin, uint8_t echo_pin, int16_t trigger_distance);

	// sends out a pulse
	bool pulse();

	// uses pulseIn to determine a distance reading
	int16_t measure_distance(unsigned long timeout);

	// called externally to check for an echo
	void checkEcho();

	// reads a value, updating `last_reading`, and returning the value
	int16_t read();

	static unsigned long global_pulse_time;

	static void update_global_pulse_time();

	static unsigned long distance_to_time(int16_t distance);

private:
	bool pulse_out = false;
	unsigned long pulse_time = 0;
	int16_t last_reading = 0;
	uint8_t trigger_pin, echo_pin;
};
