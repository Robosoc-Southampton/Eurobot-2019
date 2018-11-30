
#pragma once

#include "Message.h"

namespace robot {
	typedef void (*ActivityCallback)();
	typedef bool (*ActivityPredicate)();

	struct Activity {
		// function to run repeatedly
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

	void setup();
	void loop();

	// checks distance sensors
	void check_distance_sensors();
	// updates motor speeds based on current settings
	void update_motor_speeds();
	// calls activity callback and cancels it if necessary
	void run_activity();
	void read_message_buffer();
}
