
#include "include/robot/test/md25.h"

MD25 md25;
bool left_encoder_okay, right_encoder_okay;

void setup() {
	Serial.begin(9600);
	robot::wait_for_connection();

	rlog("Setting up MD25");

	md25.setup();

	rlog("MD25 set up");

	md25.testEncoders(&left_encoder_okay, &right_encoder_okay);	

	if (!left_encoder_okay) rlogf("Left encoder not readable");
	if (!right_encoder_okay) rlogf("Right encoder not readable");
}

void loop() {
	md25.setLeftMotorSpeed(200);
	md25.setRightMotorSpeed(200);
}
