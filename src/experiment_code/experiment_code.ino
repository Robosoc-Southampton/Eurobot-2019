
#include <Servo.h>

Servo servo;
int pos = 0;
int upper = 120;
int lower = 80;

#define LEDS_PIN 12

void setup() {
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LEDS_PIN, OUTPUT);
	pinMode(8, INPUT);
	servo.attach(9);
	delay(50);

	if (digitalRead(8)) {
		upper = 120;
		lower = 80;
	}
	else {
		upper = 140;
		lower = 100;
	}

	digitalWrite(LED_BUILTIN, HIGH);
	while (!Serial.available());
	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(LEDS_PIN, HIGH);
}

void loop() {
	for (pos = lower; pos <= upper; pos += 1) {
		servo.write(pos);
		delay(9);
	}

	for (pos = upper; pos >= lower; pos -= 1) {
		servo.write(pos);
		delay(9);
	}
}
