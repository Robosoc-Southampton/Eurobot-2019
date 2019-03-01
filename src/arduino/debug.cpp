
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

		void logf(const __FlashStringHelper* message, const char* function) {
			PGM_P p = reinterpret_cast<PGM_P>(message);
			size_t message_len; for (message_len = 0; pgm_read_byte(p + message_len); ++message_len);
			size_t function_len = strlen(function);
			uint16_t len = message_len + function_len + 5 + 2; // + 5 + 2 bytes for the " [in " and "]\n" strings
			unsigned char next_char;

			robot::send_message('l', len);

			while (next_char = pgm_read_byte(p++)) {
			    Serial.write(next_char);
			}

			Serial.print(" [in "); // TODO: should check that this is actually writing 5 bytes, not 6 (including \0)
			Serial.write(function, function_len);
			Serial.print("]\n"); // TODO: should check that this is actually writing 2 bytes, not 3 (including \0)
		}

		void logi(const int16_t message, const char* function) {
			char str[8] = {};
			itoa(message, str, 10);
			log(str, function);
		}

		void error(const char* error_message, const char* function) {
			log(error_message, function);
			pinMode(LED_BUILTIN, OUTPUT);
			
			while (1) { rblink(200) };
		}
	}
}
