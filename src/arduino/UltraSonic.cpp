
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

unsigned long UltraSonic::measureDistance(unsigned long timeout) {
	if (micros() - pulse_time < ULTRASONIC_MIN_DELAY)
		return last_reading;

	rlogf("Hello :)");
	unsigned long duration = pulseIn(echo_pin, HIGH, timeout);
	rlogf("Hi");

	if (duration)
		return last_reading = duration * 343ul / 2000ul;

	return last_reading = ULONG_MAX;
}

void UltraSonic::checkEcho() {
	if (!pulse_out) return;
	unsigned long delta_time;

	// rlogf("I am also here");

	// sourced from Arduino digitalRead function
	// https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring_digital.c#L165-L179
	// PWM turn-off not required as pins only used for digital input
	uint8_t bit = digitalPinToBitMask(echo_pin);
	uint8_t port = digitalPinToPort(echo_pin);

	if (*portInputRegister(port) & bit) {
		delta_time = micros() - pulse_time;
		last_reading = (int16_t) (delta_time * 343ul / 2000ul);
		pulse_out = false;
	}
}

int16_t UltraSonic::read() {
	pulse();
	return last_reading;
}

static unsigned long UltraSonic::distanceToTime(int16_t distance) {
	return (unsigned long) distance * 2000ul / 343ul;
}
