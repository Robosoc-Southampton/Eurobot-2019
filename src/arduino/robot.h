
#pragma once

#include "Message.h"

namespace robot {
	void setup();
	void loop();

	// checks distance sensors
	void check_distance_sensors();
	void update_motor_speeds();
	void run_activity();
	void read_message_buffer();
}
