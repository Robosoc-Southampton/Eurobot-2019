
#include "include/robot/test/md25ticks.h"

MD25 md25;

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	robot::wait_for_connection();

	md25.setup();

	rlog("MD25 set up");
	rlog("Reading left encoder");
}

void loop() {
	const char str[6] = {0, 0, 0, 0, 0, 0};

	itoa(md25.readLeftEncoder(), (char*) str, 10);
	rlog(&str[0]);
}
