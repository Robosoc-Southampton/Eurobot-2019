
#pragma once

#include <Arduino.h>

class SharpIR {
public:
	uint8_t pin;
	int16_t last_reading;
	bool enabled;

	SharpIR(uint8_t pin);

	// reads a value, updating `last_reading`, and returning the value
	int16_t read();

};
