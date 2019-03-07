
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
