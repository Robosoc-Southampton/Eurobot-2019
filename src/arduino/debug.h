
#include <arduino.h>

// use rerror(message) to error with a message
#define rerror(s) robot::debug::error(s, __FUNCTION__);

// use rassert(condition, message) to error with a message if `condition` is false
#define rassert(v, s) robot::debug::assert(v, s, __FUNCTION__);

// inline __attribute__((always_inline))

namespace robot {
	namespace debug {
		void error(const char* error_message, const char* function);

		bool assert(bool value, const char* error_message, const char* function);
		uint8_t assert(uint8_t value, const char* error_message, const char* function);
	}
}
