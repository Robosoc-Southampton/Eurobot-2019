
#pragma once

#include <arduino.h>

#define ACTIVITY3(name, c, t) \
Activity name;\
void name##_callback();\
void name##_setup() {\
	name.timeout = t;\
	name.cooldown = c;\
	name.callback = &name##_callback;\
}\
void name##_callback()
#define ACTIVITY2(name, cooldown) ACTIVITY3(name, cooldown, 0)
#define GET_ACTIVITY_MACRO(_1,_2,_3,NAME,...) NAME
#define ACTIVITY(...) GET_ACTIVITY_MACRO(__VA_ARGS__, ACTIVITY3, ACTIVITY2)(__VA_ARGS__)

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
