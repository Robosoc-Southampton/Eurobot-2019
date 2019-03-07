
#include "include/robot/test/md25.h"

MD25 md25;

void setup() {
	Serial.begin(9600);
	robot::wait_for_connection();

	rlog("Setting up MD25");

	md25.setup();

	rlog("MD25 set up");
}

void loop() {
	md25.setLeftMotorSpeed(200);
	md25.setRightMotorSpeed(200);
	md25.readLeftEncoder();
	md25.readRightEncoder();

	rlog("Read the encoder values");
}
