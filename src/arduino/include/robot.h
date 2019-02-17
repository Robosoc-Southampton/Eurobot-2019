
// for inlining:
//  inline __attribute__((always_inline))

#pragma once

#include <Arduino.h>
#include "activities.h"
#include "collision.h"
#include "debug.h"
#include "drive.h"
#include "messages.h"

typedef uint16_t (*ReadComponentValue)(uint16_t);

namespace robot {
	extern ReadComponentValue component_value_reader;

	// sets the function to read a component value
	// this function should not be significantly blocking (no delay()s, pretty much)
	void set_component_value_reader(ReadComponentValue reader);

	// sets up and validates the configuration of the robot
	void setup();

	// runs the update loop
	void loop();
}
