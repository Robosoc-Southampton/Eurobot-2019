
#include "Message.h"

Message::Message(uint8_t bytes[]) {
	command = (char) bytes[0];
	payload = ((uint16_t) bytes[1] << 8) + (uint16_t) bytes[2];
}
