
#include "include/messages.h"

Message *msgptr = nullptr;

Message::Message(uint8_t bytes[]) {
	opcode = (char) bytes[0];
	payload = ((int16_t) bytes[1] << 8) + (int16_t) bytes[2];
}

Message::Message() { /* do nothing */ }

namespace robot {

	char peek_next_opcode() {
		if (Serial.available() < MESSAGE_SIZE) return '\0';
		return (char) Serial.peek();
	}

	Message *read_message_buffer() {
		uint8_t bytes[MESSAGE_SIZE];

		if (Serial.available() < MESSAGE_SIZE) return nullptr;

		for (uint8_t i = 0; i < MESSAGE_SIZE; ++i) {
			bytes[i] = Serial.read();
		}

		if (msgptr == nullptr) msgptr = (Message*) malloc(sizeof(Message));
		*msgptr = Message(bytes);
		
		return msgptr;
	}

	void wait_for_connection() {
		while (peek_next_opcode() == '\0') {
			rblink(500);
		}
		read_message_buffer();
	}

	void send_message(char opcode, int16_t payload) {
		Serial.write(opcode);
		Serial.write((byte) (payload));
		Serial.write((byte) (payload >> 8));
	}

}
