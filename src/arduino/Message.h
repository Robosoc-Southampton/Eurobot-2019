
#pragma once

#include <arduino.h>

#define MESSAGE_SIZE sizeof(Message)

struct Message {
	char command;
	uint16_t payload;

	Message(uint8_t bytes[3]);
};
