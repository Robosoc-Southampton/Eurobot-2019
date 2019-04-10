
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
	rlogf("Use `T angle` to rotate stepper");
	rlogf("Use `P|S|G angle` to set servo angle (primary, secondary, grabber)");
	rlogf("Use `D angle` to set stepper angle");
	rlogf("Use `R 0-3` to get state of servos");
	rlogf("Use `K 1-3` to toggle which servos are active");
}

void loop() {
	robot::update_message_buffer();

	if (robot::peek_next_opcode() == 'K' || robot::peek_next_opcode() == 'k') {
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

	if (robot::peek_next_opcode() == 'P' || robot::peek_next_opcode() == 'p') {
		Message *message = robot::read_message_buffer();
		rlogf("Moving primary servo");
		primaryServo.write(message->payload);
	}

	if (robot::peek_next_opcode() == 'S' || robot::peek_next_opcode() == 's') {
		Message *message = robot::read_message_buffer();
		rlogf("Moving secondary servo");
		secondaryServo.write(message->payload);
	}

	if (robot::peek_next_opcode() == 'G' || robot::peek_next_opcode() == 'g') {
		Message *message = robot::read_message_buffer();
		rlogf("Moving grabber");
		grabberServo.write(message->payload);
	}

	if (robot::peek_next_opcode() == 'T' || robot::peek_next_opcode() == 't') {
		Message *message = robot::read_message_buffer();
		rlogf("Turning stepper for arm");
		rlogi(message->payload);

		armStepper.step(message->payload);
	}

	if (robot::peek_next_opcode() == 'D' || robot::peek_next_opcode() == 'd') {
		Message *message = robot::read_message_buffer();
		
		if (primary_enabled) { rlogf("Turning primary"); primaryServo.write(message->payload); }
		if (secondary_enabled) { rlogf("Turning secondary"); secondaryServo.write(message->payload); }
		if (grabber_enabled) { rlogf("Turning grabber"); grabberServo.write(message->payload); }
	}

	if (robot::peek_next_opcode() == 'R' || robot::peek_next_opcode() == 'r') {
		Message *message = robot::read_message_buffer();

		switch (message->payload) {
			case 0:
				rlog(primary_enabled ? "Primary enabled" : "Primary NOT enabled");
				rlog(secondary_enabled ? "Secondary enabled" : "Secondary NOT enabled");
				rlog(grabber_enabled ? "Grabber enabled" : "Grabber NOT enabled");
				break;
			case 1:
				rlogi(primaryServo.read());
				break;
			case 2:
				rlogi(secondaryServo.read());
				break;
			case 3:
				rlogi(grabberServo.read());
				break;
		}
	}

	if (robot::peek_next_opcode()) {
		Message *message = robot::read_message_buffer();
		rlogf("Unknown command");
	}

	delay(100);
}
