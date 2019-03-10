
#include "include/robot/test/md25ticks.h"

MD25 md25;
bool left_encoder_okay, right_encoder_okay;

void setup() {
	Serial.begin(9600);
	robot::wait_for_connection();

	md25.setup();

	rlog("MD25 set up");

	md25.testEncoders(&left_encoder_okay, &right_encoder_okay);	

	if (!left_encoder_okay) rlogf("Left encoder not readable");
	if (!right_encoder_okay) rlogf("Right encoder not readable");
	if (!left_encoder_okay || !right_encoder_okay) rerrorf("Failed to read encoder values");
	
	rlog("Reading left encoder");
}

void loop() {
	Message *message;

	robot::update_message_buffer();

	if ((message = robot::read_message_buffer()) != nullptr) {
		rlogf("Reset encoders");
		md25.resetEncoders();
	}

	rlogi(md25.readLeftEncoder());
	rlogi(md25.readRightEncoder());

	delay(500);
}
