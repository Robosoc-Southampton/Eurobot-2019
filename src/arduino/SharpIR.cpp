
#include "include/component/SharpIR.h"

SharpIR::SharpIR(uint8_t pin) {
	this->pin = pin;
}

int16_t SharpIR::read() {
	last_reading = analogRead(pin);
	return last_reading;
}
