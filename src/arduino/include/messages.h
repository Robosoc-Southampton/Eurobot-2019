
#pragma once

#include <Arduino.h>

#define MESSAGE_SIZE 3

struct Message {
	char opcode;
	int16_t payload;

	Message(uint8_t bytes[3]);
	Message();
};

namespace robot {
	// gets the next opcode without consuming any input
	// returns 0 if no message is available
	char peek_next_opcode();

	// checks for a message from the message buffer and acts on it if possible
	Message *read_message_buffer();

	// sends a message to the pi
	void send_message(char opcode, int16_t payload);
}
