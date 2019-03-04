
/* debug.h (internal)
 *
 * This file contains code related to debugging and logging
 * - functions used by `robot.*`
 *
 * To log a fixed debug message, use `rlogfd("<message>");`.
 */

#pragma once

#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "messages.h"

#define DEBUG true

// use rlog(message) to log a message
// 'f' variants use flash strings to save runtime memory usage, and must be used with a constant string
// 'd' variants only log in 'DEBUG' mode, saving runtime memory usage for proper runs
#define rlog(s) robot::debug::log(s, __FUNCTION__)
#define rlogd(s) if (DEBUG) rlog(s)
#define rlogf(s) robot::debug::logf(F(s), __FUNCTION__)
#define rlogfd(s) if (DEBUG) rlogf(s)
#define rlogi(i) robot::debug::logi(i, __FUNCTION__)
#define rlogid(i) if (DEBUG) rlog(i)

// use rerror(message) to error with a message
// 'f' variants use flash strings to save runtime memory usage, and must be used with a constant string
#define rerror(s) robot::debug::error(s, __FUNCTION__)
#define rerrord(s) if (DEBUG) rerror(s)
#define rerrorf(s) robot::debug::errorf(F(s), __FUNCTION__)
#define rerrorfd(s) if (DEBUG) rerrorf(s)

// use rassert(condition, message) to error with a message if `condition` is false
// 'f' variants use flash strings to save runtime memory usage, and must be used with a constant string
#define rassert(v, s) if (!(v)) rerror(s)
#define rassertd(v, s) if (DEBUG) { rassert(v, s); }
#define rassertf(v, s) if (!(v)) rerrorf(s)
#define rassertfd(v, s) if (DEBUG) { rassertf(v, s); }

// blinks the LED with a given interval
// note that usage of this function expands into multiple statements, so if references in a block, must be enclosed in `{}`
#define rblink(c, d) pinMode(LED_BUILTIN, OUTPUT); while (c) { digitalWrite(LED_BUILTIN, HIGH); delay(d/2); digitalWrite(LED_BUILTIN, LOW); delay(d/2); }

namespace robot {
	namespace debug {
		// use of these functions directly is deprecated
		// use rlog*, rerror, and rassert macros defined above
		void log(const char* message, const char* function);
		void logf(const __FlashStringHelper* message, const char* function);
		void logi(const int16_t message, const char* function);
		void error(const char* error_message, const char* function);
		void errorf(const __FlashStringHelper* error_message, const char* function);
	}
}
