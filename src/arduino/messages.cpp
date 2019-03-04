
#include "include/messages.h"

Message *msgptr = nullptr;

Message::Message() { /* do nothing */ }

namespace robot {
	const uint8_t message_buffer[MESSAGE_BUFFER_SIZE * 3] = {};
	Message *message_buffer_start_ptr = (Message*) &message_buffer[0],
	        *message_buffer_end_ptr = (Message*) &message_buffer[0],
	        *message_buffer_eof_ptr = &((Message*) &message_buffer[0])[MESSAGE_BUFFER_SIZE];
	size_t message_buffer_length = 0;
	int16_t message_buffer_consume_length = 0;
	bool message_buffer_consuming = false;
	bool is_message_buffer_valid = true;

	void invalidate_message_buffer(int16_t distance_travelled) {
		// TODO: roll back the buffer start pointer to the last forward command, update the forward distance, and push it to the start of the buffer
		//  hacky af
		is_message_buffer_valid = false;
	}

	void validate_message_buffer() {
		is_message_buffer_valid = true;
	}

	void update_message_buffer() {
		if (Serial.available() < MESSAGE_SIZE)
			return;
		
		uint8_t bytes[MESSAGE_SIZE];
		uint8_t *bytes_ptr = &bytes[0];

		for (uint8_t i = MESSAGE_SIZE; i; --i)
			*(bytes_ptr++) = Serial.read();

		if (bytes[0] == 'M') {
			switch (*((int16_t*) (bytes + 1))) {
				case 2:
					message_buffer_consuming = true;
					message_buffer_consume_length = 0;

					rlogfd("Start of message batch");

					break;
				case 3:
					message_buffer_consuming = true;
					message_buffer_length += message_buffer_consume_length;
					message_buffer_consume_length = 0;
					send_message('M', 5);

					rlogfd("Break in message batch");

					break;
				case 4:
					message_buffer_consuming = false;
					message_buffer_length += message_buffer_consume_length;
					message_buffer_consume_length = 0;

					rlogfd("End of message batch");

					break;
				case 1: case 7:
					message_buffer_start_ptr = (Message*) &message_buffer[0];
					message_buffer_end_ptr = message_buffer_start_ptr;
					message_buffer_length = 0;
					message_buffer_consume_length = 0;
					message_buffer_consuming = false;
					is_message_buffer_valid = true;

					rlogfd("Resetting message batch");

					break;
				default:
					rlogf("Unknown message command");
					rlogid(*((int16_t*) (bytes + 1)));

					break;
			}
		}
		else if (message_buffer_end_ptr != message_buffer_eof_ptr) {
			*(message_buffer_end_ptr++) = *(Message*) bytes;

			if (message_buffer_consuming) {
				++message_buffer_consume_length;
				rlogfd("Message read into consume message buffer");
			}
			else {
				++message_buffer_length;
				rlogfd("Message read directly into message buffer");
			}
		}
		else {
			rlogf("Overflow of message buffer");

			while (1) { rblink(1000) }
		}
	}

	char peek_next_opcode() {
		if (message_buffer_length == 0) return '\0';
		if (!is_message_buffer_valid) return '\0';
		return *((char*) message_buffer_start_ptr);
	}

	Message *read_message_buffer() {
		if (!is_message_buffer_valid)
			return nullptr;

		if (message_buffer_length) {
			message_buffer_length--;
			return message_buffer_start_ptr++;
		}

		return nullptr;
	}

	void wait_for_connection() {
		send_message('M', 0);
		rlogf("Waiting for connection");

		while (Serial.available() < 3) {
			rblink(500);
		}

		update_message_buffer();
		read_message_buffer();

		rlogf("Connection established");
	}

	void send_message(char opcode, int16_t payload) {
		Serial.write(opcode);
		Serial.write((byte) (payload));
		Serial.write((byte) (payload >> 8));
	}

}
