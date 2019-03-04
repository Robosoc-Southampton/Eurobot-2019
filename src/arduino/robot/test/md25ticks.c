
#include "include/robot/test/md25ticks.h"

MD25 md25;

void setup() {
	Serial.begin(9600);
	robot::wait_for_connection();

	md25.setup();

	rlog("MD25 set up");
	rlog("Reading left encoder");
}

void loop() {
	rlogi(md25.readLeftEncoder());
}
