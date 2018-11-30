
#pragma once

#include <arduino.h>

struct Activity;

typedef void (*ActivityCallback)();
typedef bool (*ActivityPredicate)();
typedef Activity* (*ActivityLookup)(uint16_t);

// an activity represents an action that can execute on the robot
// note that all times related to activities are in microseconds
struct Activity {
	// function to run when activity is triggered
	// the function should not be significantly blocking (no delay()s, pretty much)
	ActivityCallback callback = nullptr;
	// predicate must return true for activity to continue
	ActivityPredicate predicate = nullptr;
	// time to wait between calling activity callback
	uint32_t cooldown = 1;
	// time to wait before stopping activity
	uint32_t timeout = 0;
};

namespace robot {
	extern Activity *current_activity;
	extern uint32_t current_activity_cooldown;
	extern uint32_t current_activity_clock;
	extern uint32_t current_activity_last_cycle;

	extern ActivityLookup lookup_activity;
	
	// sets the function to lookup an activity
	void set_activity_lookup(ActivityLookup lookup);

	// calls activity callback and cancels it if necessary
	void run_activity();
	// starts an activity running
	void start_activity(Activity* activity);
}
