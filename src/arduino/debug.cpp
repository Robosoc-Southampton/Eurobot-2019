
#include "include/debug.h"

namespace robot {
	namespace debug {
		void log(const char* message, const char* function) {
			size_t message_len = strlen(message);
			size_t function_len = strlen(function);
			uint16_t len = message_len + function_len + 5 + 2; // + 5 + 2 bytes for the " [in " and "]\n" strings

			robot::send_message('l', len);
			Serial.write(message, message_len);
			Serial.print(" [in "); // TODO: should check that this is actually writing 5 bytes, not 6 (including \0)
			Serial.write(function, function_len);
			Serial.print("]\n"); // TODO: should check that this is actually writing 2 bytes, not 3 (including \0)
		}

		void error(const char* error_message, const char* function) {
			log(error_message, function);
			pinMode(LED_BUILTIN, OUTPUT);
			
			while (1) {
				digitalWrite(LED_BUILTIN, HIGH);
				delay(200);
				digitalWrite(LED_BUILTIN, LOW);
				delay(200);
			}
		}

		bool assert(bool value, const char* error_message, const char* function) {
			if (!value) {
				robot::debug::error(error_message, function);
			}

			return value;
		}
	}
}
