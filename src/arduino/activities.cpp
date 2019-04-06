
#include "include/activities.h"

namespace robot {

	Activity *current_activity = nullptr;
	uint32_t current_activity_cooldown = 0;
	uint32_t current_activity_clock = 0;
	uint32_t current_activity_last_cycle = 0;
	uint16_t current_activity_call_countdown = 0;

	ActivityLookup lookup_activity = nullptr;

	void set_activity_lookup(ActivityLookup lookup) {
		lookup_activity = lookup;
	}

	void stop_current_activity() {
		if (current_activity->stop != nullptr) {
			(*current_activity->stop)();
		}

		current_activity = nullptr;
	}
	
	void run_activity() {
		if (current_activity == nullptr) return;

		uint32_t current_time = micros();
		uint32_t delta_time = current_time - current_activity_last_cycle;

		// update clock and cycle
		current_activity_clock     += delta_time;
		current_activity_last_cycle = current_time;

		// if activity predicate exists and returns false, stop activity
		if (current_activity->predicate != nullptr && !(*current_activity->predicate)()) {
			rlogfd("Stopping activity due to predicate");
			stop_current_activity();
			return;
		}

		// while cooldown is satisfied
		while (current_activity_call_countdown && current_activity_clock > current_activity->cooldown) {
			// run the activity
			current_activity->callback();
			// decrement the countdown and update clock
			--current_activity_call_countdown;
			current_activity_clock -= current_activity->cooldown;
		}

		if (!current_activity_call_countdown) {
			rlogfd("Stopping activity due to countdown");
			stop_current_activity();
		}
	}

	void start_activity(Activity* activity) {
		rlogfd("Starting activity");
		// set the current activity
		current_activity = activity;
		// set the activity configuration
		current_activity_cooldown = activity->cooldown;
		// reset the clock
		current_activity_clock = 0;
		// reset the last cycle
		current_activity_last_cycle = micros();
		// set the countdown
		current_activity_call_countdown = activity->count;

		// run the activity's init function if applicable
		if (current_activity->init != nullptr) {
			rlogfd("Running activity init function");
			(*current_activity->init)();
		}
	}
	
}
