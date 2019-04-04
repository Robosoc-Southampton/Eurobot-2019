
#include "include/robot/test/arm.h"

Stepper armStepper(200, ARM_STEPPER_1, ARM_STEPPER_2, ARM_STEPPER_3, ARM_STEPPER_4);
Servo primaryServo, secondaryServo, grabberServo;

bool primary_enabled = true, secondary_enabled = false, grabber_enabled = false;

void setup() {
	Serial.begin(9600);

	armStepper.setSpeed(50);
	
	primaryServo.attach(SERVO_PRIMARY);
	secondaryServo.attach(SERVO_SECONDARY);
	grabberServo.attach(SERVO_GRABBER);

	primaryServo.write(90);
	secondaryServo.write(50);
	grabberServo.write(90);

	robot::wait_for_connection();

	rlogf("Setup complete");
}

void loop() {
	robot::update_message_buffer();

	if (robot::peek_next_opcode() == 'K') {
		Message *message = robot::read_message_buffer();

		switch (message->payload) {
			case 1: primary_enabled = !primary_enabled; break;
			case 2: secondary_enabled = !secondary_enabled; break;
			case 3: grabber_enabled = !grabber_enabled; break;
		}

		rlog(primary_enabled ? "Primary enabled" : "Primary NOT enabled");
		rlog(secondary_enabled ? "Secondary enabled" : "Secondary NOT enabled");
		rlog(grabber_enabled ? "Grabber enabled" : "Grabber NOT enabled");
	}

	if (robot::peek_next_opcode() == 'S') {
		Message *message = robot::read_message_buffer();

		primary_enabled = message->payload == 1;
		secondary_enabled = message->payload == 2;
		grabber_enabled = message->payload == 3;

		rlog(primary_enabled ? "Primary enabled" : "Primary NOT enabled");
		rlog(secondary_enabled ? "Secondary enabled" : "Secondary NOT enabled");
		rlog(grabber_enabled ? "Grabber enabled" : "Grabber NOT enabled");
	}

	if (robot::peek_next_opcode() == 'T') {
		Message *message = robot::read_message_buffer();
		rlogf("Turning stepper for arm");
		rlogi(message->payload);

		armStepper.step(message->payload);
	}

	if (robot::peek_next_opcode() == 'D') {
		Message *message = robot::read_message_buffer();
		
		if (primary_enabled) { rlogf("Turning primary"); primaryServo.write(message->payload); }
		if (secondary_enabled) { rlogf("Turning secondary"); secondaryServo.write(message->payload); }
		if (grabber_enabled) { rlogf("Turning grabber"); grabberServo.write(message->payload); }
	}

	if (robot::peek_next_opcode() == 'R') {
		Message *message = robot::read_message_buffer();

		switch (message->payload) {
			case 0:
				rlog(primary_enabled ? "Primary enabled" : "Primary NOT enabled");
				rlog(secondary_enabled ? "Secondary enabled" : "Secondary NOT enabled");
				rlog(grabber_enabled ? "Grabber enabled" : "Grabber NOT enabled");
				break;
			case 1: rlogid(primaryServo.read()); break;
			case 2: rlogid(secondaryServo.read()); break;
			case 3: rlogid(grabberServo.read()); break;
		}
	}

	delay(100);
}
