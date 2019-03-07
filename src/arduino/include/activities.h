
/* activity.h
 *
 * This file contains code related to activities
 * - the Activity type
 * - macros for defining activities
 * - setup functions used in entry point setup
 * - setup/loop functions used by `robot.*`
 *
 * Note that only the macros need to be used for defining activities. See below code declarations.
 *
 * An activity is an action that will run over a period of time.
 * Activities are invoked using 'do' messages.
 * Activities are looked-up using the callback set with `robot::set_activity_lookup()`.
 * Only one activity may be active at once, so queueing two activities, e.g. 'do 1; do 0', will halt reading further messages until the first has finished.
 */

#pragma once

#include <Arduino.h>
#include "debug.h"

struct Activity;

// a callback that runs the activity
typedef void (*ActivityCallback)();
// a callback that returns true if the activity should continue
typedef bool (*ActivityPredicate)();
// a callback that returns the activity from an activity ID
typedef Activity* (*ActivityLookup)(uint16_t);

// an activity represents an action that can execute on the robot
// note that all times related to activities are in microseconds
struct Activity {
	// function to run when activity is triggered
	// note that this function calls irrespective of the predicate
	ActivityCallback init = nullptr;
	// function to run while activity is running
	// the function should not be significantly blocking (no delay()s, pretty much)
	ActivityCallback callback = nullptr;
	// predicate must return true for activity to continue
	ActivityPredicate predicate = nullptr;
	// time to wait between calling activity callback
	uint32_t cooldown = 1;
	// total number of times to run activity callback
	uint16_t count = 0;
};

namespace robot {
	extern Activity *current_activity;
	extern uint32_t current_activity_cooldown;
	extern uint32_t current_activity_clock;
	extern uint32_t current_activity_last_cycle;
	extern uint16_t current_activity_call_countdown;

	extern ActivityLookup lookup_activity;
	
	// sets the function to lookup an activity
	// should be called prior to robot::setup()
	void set_activity_lookup(ActivityLookup lookup);

	// starts an activity running
	void start_activity(Activity* activity);

	// calls activity callback and cancels it if necessary (internal)
	void run_activity();
}

/* The code below defines macros allowing the following:
 *
 * to define an activity...
 *
 * 	ACTIVITY(name, cooldown=C, count=T) {
 * 		// code to run
 * 	}
 *
 * or
 * 
 * 	ACTIVITY(name, cooldown=C) {
 * 		// code to run
 * 	}
 *
 * then, to assign an init function
 *
 * 	INIT(name) {
 * 		// code to run
 * 	}
 *
 * then, to assign a predicate...
 * 
 * 	PREDICATE(name) {
 * 		return true;
 * 	}
 *
 * then, to reference it...
 *
 * 	Activity *the_activity = ACTIVITY(name)
 *
 */

#define ACTIVITY3(name, c, n) \
Activity name;\
void name##_callback();\
struct name##_init_struct {\
	name##_init_struct() {\
		name.n;\
		name.c;\
		name.callback = &name##_callback;\
	}\
};\
name##_init_struct name##_init_struct_instance;\
void name##_callback()
#define ACTIVITY2(name, cooldown) ACTIVITY3(name, cooldown, count=0)
#define ACTIVITY1(ref) &ref
#define GET_ACTIVITY_MACRO(_1,_2,_3,NAME,...) NAME
#define ACTIVITY(...) GET_ACTIVITY_MACRO(__VA_ARGS__, ACTIVITY3, ACTIVITY2, ACTIVITY1)(__VA_ARGS__)

#define PREDICATE(name) \
bool name##_predicate();\
struct name##_init_predicate_struct {\
	name##_init_predicate_struct() {\
		name.predicate = &name##_predicate;\
	}\
};\
name##_init_predicate_struct name##_init_predicate_struct_instance;\
bool name##_predicate()

#define INIT(name) \
void name##_init();\
struct name##_init_init_struct {\
	name##_init_init_struct() {\
		name.init = &name##_init;\
	}\
};\
name##_init_init_struct name##_init_init_struct_instance;\
void name##_init()
