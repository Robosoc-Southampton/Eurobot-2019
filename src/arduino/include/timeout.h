
/* timeout.h
 *
 * This file contains code related to pull cord registration and
 * stopping the robot after a timeout
 */

#pragma once

#include <Arduino.h>
#include "debug.h"
#include "drive.h"

namespace robot {
	const int32_t TIMEOUT = 98000;

	bool wait_for_pullcord(uint8_t pin);
	void notify_start();
	void check_timeout();
}
