
#include "include/component/UltraSonic.h"

unsigned long UltraSonic::global_pulse_time = 0;

static void UltraSonic::update_global_pulse_time() {
	unsigned long now = micros();

	while (now - UltraSonic::global_pulse_time >= ULTRASONIC_MIN_DELAY)
		UltraSonic::global_pulse_time += ULTRASONIC_MIN_DELAY;
}

UltraSonic::UltraSonic(uint8_t trigger_pin, uint8_t echo_pin, int16_t trigger_distance) {
	this->trigger_pin = trigger_pin;
	this->echo_pin = echo_pin;
	this->trigger_distance = trigger_distance;

	pinMode(trigger_pin, OUTPUT);
	pinMode(echo_pin, INPUT);
}

bool UltraSonic::pulse() {
	if (pulse_out && pulse_time == UltraSonic::global_pulse_time)
		return false;

	digitalWrite(trigger_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigger_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigger_pin, LOW);

	pulse_out = true;
	pulse_time = UltraSonic::global_pulse_time;

	return true;
}

int16_t UltraSonic::measure_distance(unsigned long timeout) {
	if (pulse_time == UltraSonic::global_pulse_time)
		return last_reading;

	unsigned long duration;

	digitalWrite(trigger_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigger_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigger_pin, LOW);

	duration = pulseIn(echo_pin, HIGH, timeout);
	pulse_time = UltraSonic::global_pulse_time;

	if (duration)
		return last_reading = duration * 343ul / 2000ul;

	return last_reading = INT_MAX;
}

void UltraSonic::checkEcho() {
	if (!pulse_out) return;
	unsigned long delta_time;

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

static unsigned long UltraSonic::distance_to_time(int16_t distance) {
	return (unsigned long) distance * 2000ul / 343ul;
}
