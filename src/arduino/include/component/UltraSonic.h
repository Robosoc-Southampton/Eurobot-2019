
#pragma once

#include <Arduino.h>
#include <limits.h>
#include "../debug.h"

#define ULTRASONIC_MIN_DELAY 1000

class UltraSonic {
public:
	int16_t trigger_distance;
	bool enabled = true;

	// creates the UltraSonic object, with the digital pins it's connected to
	UltraSonic(uint8_t trigger_pin, uint8_t echo_pin, int16_t trigger_distance);

	// sends out a pulse
	bool pulse();

	// uses pulseIn to determine a distance reading
	unsigned long measureDistance(unsigned long timeout);

	// called externally to check for an echo
	void checkEcho();

	// reads a value, updating `last_reading`, and returning the value
	int16_t read();

	static unsigned long distanceToTime(int16_t distance);

private:
	bool pulse_out;
	unsigned long pulse_time;
	uint8_t trigger_pin, echo_pin;
	int16_t last_reading = 0;
};
