
#include "include/component/SharpIR.h"

SharpIR::SharpIR(uint8_t pin) {
	this->pin = pin;
}

int16_t SharpIR::read() {
	last_reading = convertVoltageToDistance(analogRead(pin));
	return last_reading;
}

int16_t SharpIR::convertVoltageToDistance(int16_t voltage) {
	return voltage; // TODO: perform actual conversion
}
