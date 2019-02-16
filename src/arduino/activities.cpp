
#include "include/activities.h"

namespace robot {

	Activity *current_activity = nullptr;
	uint32_t current_activity_cooldown = 0;
	uint32_t current_activity_clock = 0;
	uint32_t current_activity_last_cycle = 0;

	ActivityLookup lookup_activity = nullptr;

	void set_activity_lookup(ActivityLookup lookup) {
		lookup_activity = lookup;
	}
	
	void run_activity() {
		if (current_activity == nullptr) return;

		uint32_t current_time = micros();
		uint32_t delta_time = current_time - current_activity_last_cycle;

		// update clock and cycle
		current_activity_clock     += delta_time;
		current_activity_last_cycle = current_time;

		// if activity has timed out then stop it
		if (current_activity_clock > current_activity->timeout && current_activity->timeout != 0) {
			current_activity = nullptr;
			return;
		}

		// if activity predicate exists and returns false, stop activity
		if (current_activity->predicate != nullptr && !(*current_activity->predicate)()) {
			current_activity = nullptr;
			return;
		}

		// if cooldown is satisfied
		if (current_activity_cooldown <= delta_time) {
			// run the activity
			current_activity->callback();
			// reset the cooldown (TODO: review this)
			current_activity_cooldown = delta_time >= current_activity->cooldown
			                          ? current_activity->cooldown
			                          : current_activity->cooldown - delta_time + current_activity_cooldown;
		}
		else {
			// otherwise, update cooldown
			current_activity_cooldown -= delta_time;
		}
	}

	void start_activity(Activity* activity) {
		// set the current activity
		current_activity = activity;
		// set the activity configuration
		current_activity_cooldown = activity->cooldown;
		// reset the clock
		current_activity_clock = 0;
		// run the callback once immediately
		(*activity->callback)();
		// reset the last cycle
		current_activity_last_cycle = micros();
	}
	
}
