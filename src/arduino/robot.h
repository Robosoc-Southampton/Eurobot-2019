
#pragma once

#include <arduino.h>
#include "Message.h"
#include "activities.h"
#include "debug.h"

typedef uint16_t (*ReadComponentValue)(uint16_t);

namespace robot {
	extern bool is_moving;

	extern ReadComponentValue component_value_reader;

	// sets the function to read a component value
	// this function should not be significantly blocking (no delay()s, pretty much)
	void set_component_value_reader(ReadComponentValue reader);

	// sets the function to lookup an activity
	void set_activity_lookup(ActivityLookup lookup);

	void setup();
	void loop();

	// checks distance sensors
	void check_distance_sensors();
	// updates motor speeds based on current settings
	void update_motor_speeds();
	// checks for a message from the message buffer and acts on it if possible
	void read_message_buffer();

	// sends a message to the pi
	void send_message(char command, uint16_t payload);
}
