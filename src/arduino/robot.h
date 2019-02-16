
#pragma once

#include <Arduino.h>
#include "activities.h"
#include "collision.h"
#include "debug.h"
#include "drive.h"

#define MESSAGE_SIZE 3

typedef uint16_t (*ReadComponentValue)(uint16_t);

struct Message {
	char command;
	uint16_t payload;

	Message(uint8_t bytes[3]);
	Message();
};

namespace robot {
	extern ReadComponentValue component_value_reader;

	// sets the function to read a component value
	// this function should not be significantly blocking (no delay()s, pretty much)
	void set_component_value_reader(ReadComponentValue reader);

	void setup();
	void loop();

	// checks for a message from the message buffer and acts on it if possible
	void read_message_buffer();

	// sends a message to the pi
	void send_message(char command, uint16_t payload);
}
