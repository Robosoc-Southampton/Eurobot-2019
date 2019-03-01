
/* messages.h
 *
 * This file contains code related to reading messages
 * - the Message type
 * - setup/loop functions used by `robot.*`
 */

#pragma once

#include <Arduino.h>
#include "debug.h"

#define MESSAGE_SIZE 3
#define MESSAGE_BUFFER_SIZE 200

struct Message {
	char opcode;
	int16_t payload;

	Message(uint8_t bytes[3]);
	Message();
};

namespace robot {
	// stores commands
	extern const uint8_t message_buffer[MESSAGE_BUFFER_SIZE * 3];
	extern Message *message_buffer_start_ptr, *message_buffer_end_ptr, *message_buffer_eof_ptr;
	extern size_t message_buffer_length;
	extern int16_t message_buffer_consume_length;
	extern bool message_buffer_consuming;

	// checks for a message from the message buffer and acts on it if possible
	void update_message_buffer();

	// gets the next opcode without consuming any input
	// returns 0 if no message is available
	char peek_next_opcode();

	// checks for a message from the message buffer and acts on it if possible
	Message *read_message_buffer();

	// waits for a connection from the controller, flashing the LED while it waits
	void wait_for_connection();

	// sends a message to the controller
	void send_message(char opcode, int16_t payload);
}
