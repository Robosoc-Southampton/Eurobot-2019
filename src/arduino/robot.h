
#pragma once

#include <arduino.h>
#include "Message.h"
#include "debug.h"

namespace robot {
	struct Activity;

	typedef void (*ActivityCallback)();
	typedef bool (*ActivityPredicate)();
	typedef uint16_t (*ReadComponentValue)(uint16_t);
	typedef Activity* (*ActivityLookup)(uint16_t);

	// an activity represents an action that can execute on the robot
	// note that all times related to activities are in microseconds
	struct Activity {
		// function to run when activity is triggered
		// the function should not be significantly blocking (no delay()s, pretty much)
		ActivityCallback callback;
		// predicate must return true for activity to continue
		ActivityPredicate predicate;
		// time to wait between calling activity callback
		uint32_t cooldown;
		// time to wait before stopping activity
		uint32_t timeout;
	};

	extern Activity *current_activity;
	extern uint32_t current_activity_cooldown;
	extern uint32_t current_activity_clock;
	extern uint32_t current_activity_last_call;

	extern bool is_moving;

	extern ReadComponentValue component_value_reader;
	extern ActivityLookup lookup_activity;

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
	// calls activity callback and cancels it if necessary
	void run_activity();
	// checks for a message from the message buffer and acts on it if possible
	void read_message_buffer();

	// sends a message to the pi
	void send_message(char command, uint16_t payload);
}
