
#include "include/component/SharpIR.h"

SharpIR::SharpIR(uint8_t pin, int16_t trigger_voltage) {
	this->pin = pin;
	this->trigger_voltage = trigger_voltage;
}

int16_t SharpIR::read() {
	last_reading = analogRead(pin);
	return last_reading;
}

int16_t SharpIR::convertVoltageToDistance(int16_t adc_input) {
	/*
	float v_reading = (((float) adc_input) / 1023) * 5;
	float distance = x_0 - (x_1 * v_reading) + (x_2 * pow(v_reading,2)) 
			- (x_3 * pow(v_reading,3)) + (x_4 * pow(v_reading,4))
			- (x_5 * pow(v_reading,5));
	*/

	float distance = adc_input;
	
	return (int16_t) distance;
}
