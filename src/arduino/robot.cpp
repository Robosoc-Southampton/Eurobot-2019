
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
	}

	void loop() {
		if (check_distance_sensors()) {
			drive::reset();
			int16_t distance = drive::get_average_distance_travelled();
			robot::send_message('c', distance);
		}

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
			case 'R': case 'K': case 'S':
				// can always read a component value or set config stuff
				is_readable = true;
				break;
			case 'M':
				rlog("Unexpected 'M' message");
				is_readable = true;
				break;
		}

		// if can't read next message, stop
		if (!is_readable)
			return;

		rlogd("There's a message");

		message = read_message_buffer();

		char msg[2] = {'\0', '\0'};
		msg[0] = message->opcode;
		rlogd((const char*) msg);

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
				switch (message->payload) {
					case 0: // skip activity 0 (do nothing activity)
						break;
					case 1000: // activity 1000 enables sensors
						rlog("Enabling all distance sensors");
						robot::enable_distance_sensors();
						break;
					case 1100: // activity 1100 disables sensors
						rlog("Disabling all distance sensors");
						robot::enable_distance_sensors();
						break;
					default: // run another activity
						if (message->payload > 1000) {
							if (message->payload < 1200) { // enable/disable a specific sensor
								if (message->payload < 1100) {
									rlog("Enabling distance sensor");
									robot::enable_distance_sensor(message->payload - 1001);
								}
								else {
									rlog("Disabling distance sensor");
									robot::disable_distance_sensor(message->payload - 1101);
								}
								break;
							}
						}
						next_activity = (*lookup_activity)(message->payload);

						if (next_activity == nullptr) {
							itoa(message->payload, (char*) &buffer[0], 10);
							rlog("Activity lookup failed");
							rlog(buffer);
							break;
						}

						start_activity(next_activity);

						break;
				}
				break;
			case 'K':
				robot::configuration::set_config_key(message->payload);
				break;
			case 'S':
				robot::configuration::set_config_value(message->payload);
				break;
			case 'R': // request
				int16_t value;

				if (message->payload > 1000 && message->payload < 1100) {
					value = robot::read_distance_sensor(message->payload - 1001);
				}
				else {
					value = (*component_value_reader)(message->payload);
				}

				robot::send_message('r', value);
				break;
		}
	}

}
