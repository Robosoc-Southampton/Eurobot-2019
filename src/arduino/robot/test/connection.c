
#include "include/robot/test/connection.h"

void setup() {
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

	robot::wait_for_connection();

	rlog("Hello");
	rlogf("Hello (flash)");
}

void loop() {
	robot::update_message_buffer();
	Message *message = robot::read_message_buffer();

	if (message != nullptr) {
		const char buffer[8] = {'0', 'x'};

		switch (message->opcode) {
			case 'F': if (message->payload >= 0) rlog("Forward"); else rlog("Backward"); break;
			case 'T': rlog("Turn"); break;
			case 'D': rlog("Do"); break;
			default:  rlog("Other"); break;
		}

		itoa(message->payload, (char*) &buffer[2], 16);
		rlog(buffer);

		robot::send_message('r', message->payload);
	}
}
