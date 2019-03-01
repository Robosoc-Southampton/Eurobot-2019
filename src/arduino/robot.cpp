
#include "include/robot.h"

namespace robot {

	ReadComponentValue component_value_reader = nullptr;

	void set_component_value_reader(ReadComponentValue reader) {
		component_value_reader = reader;
	}

	void setup() {
		pinMode(LED_BUILTIN, OUTPUT);
		MD25 *md25 = new MD25();

		robot::wait_for_connection();
		rlogf("Connection established");

		rlogf("Setting up MD25");
		md25->setup();
		rlogf("MD25 set up");

		robot::drive::set_md25(md25);

		rassert(component_value_reader != nullptr, rlogf("Component value reader not set"));
		rassert(lookup_activity != nullptr, rlogf("Activity lookup not set"));
		rassert(distance_sensors != nullptr, rlogf("Distance sensors not set"));
		rassert(drive::md25 != nullptr, rlogf("MD25 allocation failed"));

		rlogf("Initialisation complete");
	}

	void loop() {
		if (drive::is_moving && check_distance_sensors()) {
			drive::stop();
			int16_t distance = drive::get_average_distance_travelled();
			robot::send_message('c', distance);
		}

		drive::update_motor_speeds();
		run_activity();
	
		char opcode;
		bool is_activity_running = current_activity != nullptr;
		bool is_readable = false;
		Message *message;

		update_message_buffer();

		if ((opcode = peek_next_opcode()) == '\0')
			return;

		// test the opcode char to determine if the message is ready to be read
		switch (opcode) {
			case 'F': case 'T': // forward and turn commands
				// can only run next movement opcode if not currently moving
				is_readable = !drive::is_moving;
				break;
			case 'D': // can only run next activity if the current one has finished
				is_readable = !is_activity_running && !drive::is_moving;
				break;
			case 'R': case 'K': case 'S':
				// can always read a component value or set config stuff
				is_readable = true;
				break;
			case 'M':
				rlogf("Unexpected 'M' message");
				is_readable = true;
				break;
		}

		// if can't read next message, stop
		if (!is_readable)
			return;

		rlogfd("There's a message");

		message = read_message_buffer();

		char msg[2] = {'\0', '\0'};
		msg[0] = message->opcode;
		rlogd((const char*) msg);

		// act on message
		consume_message(message);
	}

	void consume_message(Message *message) {
		switch (message->opcode) {
			case 'F': // forward
				robot::distance_sensor_enabled_mask = true;
				drive::forward(message->payload);
				break;
			case 'T': // turn
				robot::distance_sensor_enabled_mask = false;
				drive::turn(message->payload);
				break;
			case 'D': // do
				perform_do_command(message->payload);
				break;
			case 'K':
				robot::configuration::set_config_key(message->payload);
				break;
			case 'S':
				robot::configuration::set_config_value(message->payload);
				break;
			case 'R': // request
				int16_t value;

				// requests to 1001-1099 are distance sensor requests
				if (message->payload > 1000 && message->payload < 1100) {
					rlogfd("Reading distance sensor");
					value = robot::read_distance_sensor(message->payload - 1001);
				}
				else {
					rlogfd("Reading component");
					value = (*component_value_reader)(message->payload);
				}

				robot::send_message('r', value);
				break;
		}
	}

	void perform_do_command(int16_t payload) {
		char failure_buffer[6] = {};
		Activity *next_activity;

		switch (payload) {
			case 0: // skip activity 0 (do nothing activity)
				break;
			case 1000: // activity 1000 enables sensors
				rlogf("Enabling all distance sensors");
				robot::enable_distance_sensors();
				break;
			case 1100: // activity 1100 disables sensors
				rlogf("Disabling all distance sensors");
				robot::enable_distance_sensors();
				break;
			default: // run another activity
				if (payload > 1000) {
					if (payload < 1200) { // enable/disable a specific sensor
						if (payload < 1100) {
							rlogf("Enabling distance sensor");
							robot::enable_distance_sensor(payload - 1001);
						}
						else {
							rlogf("Disabling distance sensor");
							robot::disable_distance_sensor(payload - 1101);
						}
						break;
					}
				}

				next_activity = (*lookup_activity)(payload);

				if (next_activity == nullptr) {
					itoa(payload, (char*) &failure_buffer[0], 10);
					rlogf("Activity lookup failed");
					rlog(failure_buffer);
					break;
				}

				start_activity(next_activity);

				break;
		}
	}

}
