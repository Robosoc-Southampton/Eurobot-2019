
#pragma once

#include <arduino.h>

// use rlog(message) to log a message
#define rlog(s) robot::debug::log(s, __FUNCTION__)

// use rerror(message) to error with a message
#define rerror(s) robot::debug::error(s, __FUNCTION__)

// use rassert(condition, message) to error with a message if `condition` is false
#define rassert(v, s) robot::debug::assert(v, s, __FUNCTION__)

// inline __attribute__((always_inline))

namespace robot {
	namespace debug {
		// use of these functions directly is deprecated
		// use rlog, rerror, and rassert macros defined above
		void log(const char* message, const char* function);
		void error(const char* error_message, const char* function);
		bool assert(bool value, const char* error_message, const char* function);
	}
}
