
#include "include/debug.h"

namespace robot {
	namespace debug {
		void error(const char* error_message, const char* function) {
			Serial.print(error_message);
			Serial.print(F(" [in "));
			Serial.print(function);
			Serial.println(F("]"));

			delay(100);
			exit(1);
		}

		bool assert(bool value, const char* error_message, const char* function) {
			if (!value) {
				robot::debug::error(error_message, function);
			}

			return value;
		}

		uint8_t assert(uint8_t value, const char* error_message, const char* function) {
			if (value == 0) {
				robot::debug::error(error_message, function);
			}

			return value;
		}
	}
}
