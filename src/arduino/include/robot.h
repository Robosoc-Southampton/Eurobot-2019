
/* robot.h
 *
 * This file contains code related to overall robot control
 * - setup functions used in entry point setup
 * - setup/loop functions called from entry point code
 */

// for inlining:
//  inline __attribute__((always_inline))

#pragma once

#include <Arduino.h>
#include "activities.h"
#include "collision.h"
#include "configuration.h"
#include "debug.h"
#include "drive.h"
#include "messages.h"
#include "timeout.h"
#include "component/MD25.h"

typedef int16_t (*ReadComponentValue)(int16_t);

namespace robot {
	extern ReadComponentValue component_value_reader;

	// sets the function to read a component value
	// this function should not be significantly blocking (no delay()s, pretty much)
	void set_component_value_reader(ReadComponentValue reader);

	// sets up and validates the configuration of the robot
	void setup();

	// runs the update loop
	void loop();

	// consume and act on next message (internal)
	void consume_message(Message *message);

	// performs a 'do' command, e.g. enabling sensors, rotating a motor, or waiting (internal)
	void perform_do_command(int16_t payload);
}
