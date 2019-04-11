
#include "include/component/UltraSonic.h"

UltraSonic::UltraSonic(uint8_t trigger_pin, uint8_t echo_pin, int16_t trigger_distance) {
	this->trigger_pin = trigger_pin;
	this->echo_pin = echo_pin;
	this->trigger_distance = trigger_distance;

	pinMode(trigger_pin, OUTPUT);
	pinMode(echo_pin, INPUT);
}

bool UltraSonic::pulse() {
	if (pulse_out && micros() - pulse_time < ULTRASONIC_MIN_DELAY)
		return false;

	digitalWrite(trigger_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigger_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigger_pin, LOW);

	pulse_out = true;
	pulse_time = micros();

	return true;
}

void UltraSonic::notifyEcho() {
	if (!pulse_out) return;

	unsigned long delta_time = micros() - pulse_time;

	last_reading = (int16_t) (delta_time * 343ul / 2000ul);
	pulse_out = false;
}

int16_t UltraSonic::read() {
	pulse();
	return last_reading;
}
