
#include "include/robot/test/sharpIR.h"

SharpIR sensor = SharpIR(A0);

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	// robot::wait_for_connection();

	rlog("Sensor connected to A0");
}

void loop() {
	const char str[23] = "Sensor reading: \0\0\0\0\0\0";

	itoa(sensor.read(), (char*) str + 16, 10);
	rlog(&str[0]);

	delay(200);
}
