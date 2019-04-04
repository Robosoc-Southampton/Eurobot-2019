
#pragma once

#include <Arduino.h>

class SharpIR {
public:
	uint8_t pin;
	int16_t last_reading = 0;
	int16_t trigger_voltage;
	bool enabled = true;

	// creates the SharpIR object, with the analogue pin it's connected to
	SharpIR(uint8_t pin, int16_t trigger_voltage);

	// reads a value, updating `last_reading`, and returning the value
	int16_t read();

	// converts a voltage into a distance
	int16_t convertVoltageToDistance(int16_t voltage);

};
