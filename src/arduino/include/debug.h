
#pragma once

#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "messages.h"

#define DEBUG true

// use rlog(message) to log a message
#define rlog(s) robot::debug::log(s, __FUNCTION__)
#define rlogd(s) if (DEBUG) robot::debug::log(s, __FUNCTION__)
#define rlogf(s) robot::debug::logf(F(s), __FUNCTION__)
#define rlogfd(s) if (DEBUG) robot::debug::logf(F(s), __FUNCTION__)

// use rerror(message) to error with a message
#define rerror(s) robot::debug::error(s, __FUNCTION__)

// use rassert(condition, message) to error with a message if `condition` is false
#define rassert(v, s) if (!(v)) s

#define rblink(d) digitalWrite(LED_BUILTIN, HIGH); delay(d/2); digitalWrite(LED_BUILTIN, LOW); delay(d/2);

namespace robot {
	namespace debug {
		// use of these functions directly is deprecated
		// use rlog, rerror, and rassert macros defined above
		void log(const char* message, const char* function);
		void logf(const __FlashStringHelper* message, const char* function);
		void error(const char* error_message, const char* function);
	}
}
