
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
#define rlogd(s) if (DEBUG) robot::debug::log(s, __FUNCTION__)
#define rlogf(s) robot::debug::logf(F(s), __FUNCTION__)
#define rlogfd(s) if (DEBUG) robot::debug::logf(F(s), __FUNCTION__)
#define rlogid(i) if (DEBUG) robot::debug::logi(i, __FUNCTION__)

// use rerror(message) to error with a message
#define rerror(s) robot::debug::error(s, __FUNCTION__)

// use rassert(condition, message) to error with a message if `condition` is false
#define rassert(v, s) if (!(v)) s

// blinks the LED with a given interval
// note that usage of this function expands into multiple statements, so if references in a block, must be enclosed in `{}`
#define rblink(d) digitalWrite(LED_BUILTIN, HIGH); delay(d/2); digitalWrite(LED_BUILTIN, LOW); delay(d/2);

namespace robot {
	namespace debug {
		// use of these functions directly is deprecated
		// use rlog*, rerror, and rassert macros defined above
		void log(const char* message, const char* function);
		void logf(const __FlashStringHelper* message, const char* function);
		void logi(const int16_t message, const char* function);
		void error(const char* error_message, const char* function);
	}
}
