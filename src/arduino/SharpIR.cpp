
#include "include/component/SharpIR.h"

SharpIR::SharpIR(uint8_t pin) {
	this->pin = pin;
}

int16_t SharpIR::read() {
	last_reading = convertVoltageToDistance(analogRead(pin));
	return last_reading;
}

int16_t SharpIR::convertVoltageToDistance(int adc_input) {
	
	int16_t v_reading = (((int16_t) adc_input) /1023) * 5;
	int16_t distance = x_0 - (x_1 * v_reading) + (x_2 * pow(v_reading,2)) 
			- (x_3 * pow(v_reading,3)) + (x_4 * pow(v_reading,4))
			- (x_5 * pow(v_reading,5));
	
	return distance;
}
