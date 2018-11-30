
#pragma once

#include <arduino.h>

#define MESSAGE_SIZE 3

struct Message {
	char command;
	uint16_t payload;

	Message(uint8_t bytes[3]);
	Message() {}
};
