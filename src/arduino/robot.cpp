
#include "include/robot.h"

namespace robot {

	ReadComponentValue component_value_reader = nullptr;

	void set_component_value_reader(ReadComponentValue reader) {
		component_value_reader = reader;
	}

	void setup() {
		rassert(component_value_reader != nullptr, "Component value reader not set");
		rassert(lookup_activity != nullptr, "Activity lookup not set");
		rassert(distance_sensors != nullptr, "Distance sensors not set");
		rassert(drive::md25 != nullptr, "Drive MD25 not set");
		rassert(configuration::radii_set, "Radii not set");
	}

	void loop() {
		check_distance_sensors();
		drive::update_motor_speeds();
		run_activity();
	
		char buffer[6] = {};
		char opcode;
		bool is_activity_running = current_activity != nullptr;
		bool is_readable = false;
		Message *message;
		Activity *next_activity;

		if ((opcode = peek_next_opcode()) == '\0')
			return;

		// test the opcode char to determine if the message is ready to be read
		switch (opcode) {
			case 'F': case 'T': case 'A': // forward, turn and align commands
				// can only run next movement opcode if not currently moving
				is_readable = !drive::is_moving;
				break;
			case 'D': // can only run next activity if the current one has finished
				is_readable = !is_activity_running;
				break;
			case 'R':
				// can always read a component value
				is_readable = true;
				break;
		}

		// if can't read next message, stop
		if (!is_readable)
			return;

		message = read_message_buffer();

		// act on message
		switch (message->opcode) {
			case 'F': // forward
				enable_distance_sensors();
				drive::forward(message->payload);
				break;
			case 'T': // turn
				disable_distance_sensors();
				drive::turn(message->payload);
				break;
			case 'A': // align
				disable_distance_sensors();
				drive::forward(-drive::ALIGN_DISTANCE);
				// TODO: spec requires to drive forward by message.payload after aligning
				break;
			case 'D': // do
				if (message->payload == 0) // skip activity 0 (do nothing activity)
					break;

				next_activity = (*lookup_activity)(message->payload);

				if (next_activity == nullptr) {
					itoa(message->payload, (char*) &buffer[0], 10);
					rlog("Activity lookup failed");
					rlog(buffer);
					break;
				}

				start_activity(next_activity);

				break;
			case 'R': // request
				uint16_t value = (*component_value_reader)(message->payload);
				send_message('r', value);
				break;
		}
	}

}
