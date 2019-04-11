
#pragma once

#include <Arduino.h>

#define ULTRASONIC_MIN_DELAY 1000

class UltraSonic {
public:
	int16_t trigger_distance;
	bool enabled = true;

	// creates the UltraSonic object, with the digital pins it's connected to
	UltraSonic(uint8_t trigger_pin, uint8_t echo_pin, int16_t trigger_distance);

	// sends out a pulse
	bool pulse();

	// called externally to register an echo
	void notifyEcho();

	// reads a value, updating `last_reading`, and returning the value
	int16_t read();

private:
	bool pulse_out;
	unsigned long pulse_time;
	uint8_t trigger_pin, echo_pin;
	int16_t last_reading = 0;
};
