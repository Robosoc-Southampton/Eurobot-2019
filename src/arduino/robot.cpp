
#include "include/robot.h"

namespace robot {

	ReadComponentValue component_value_reader = nullptr;
	bool __activity_duration_limit_warning = false;

	void set_component_value_reader(ReadComponentValue reader) {
		component_value_reader = reader;
	}

	void setup() {
		MD25 *md25 = new MD25();
		bool left_encoder_okay, right_encoder_okay;

		robot::wait_for_connection();

		rlogf("Setting up MD25");
		md25->setup();
		rlogf("MD25 set up");

		rlogf("Battery voltage is:");
		rlogi(md25->getBatteryVoltage());

		if (md25->getBatteryVoltage() < 110)
			rerrorf("Battery voltage too low!");

		md25->testEncoders(&left_encoder_okay, &right_encoder_okay);

		if (!left_encoder_okay) rlogf("Left encoder not readable");
		if (!right_encoder_okay) rlogf("Right encoder not readable");
		if (!left_encoder_okay || !right_encoder_okay) rerrorf("Failed to read encoder values");

		robot::drive::set_md25(md25);
		robot::setup_collision();

		rassertf(component_value_reader != nullptr, "Component value reader not set");
		rassertf(lookup_activity != nullptr, "Activity lookup not set");
		rassertf(distance_sensors != nullptr, "Distance sensors not set");
		rassertf(drive::md25 != nullptr, "MD25 allocation failed");

		rlogf("Initialisation complete");
	}

	void loop() {
		int sensor;

		robot::check_timeout();
		UltraSonic::update_global_pulse_time();

		if ((sensor = check_distance_sensors()) != -1) {
			if (drive::is_moving && drive::is_moving_forward && robot::is_message_buffer_valid) {
				int16_t distance = drive::get_average_distance_travelled();
				drive::stop();
				robot::invalidate_message_buffer(distance);
				robot::send_message('c', distance);
				drive::target_left_encoder_value = drive::target_right_encoder_value = 0;
				rlogfd("Triggered sensor:");
				rlogid(sensor);
			}
		}
		else if (!robot::is_message_buffer_valid) {
			rlogfd("Revalidating message buffer");
			robot::validate_message_buffer();
		}

		drive::update_motor_speeds();

		long st = millis();
		run_activity();
		if (!__activity_duration_limit_warning && millis() - st > 5) {
			__activity_duration_limit_warning = false;
			rlogf("Warning: activity duration exceeded 5ms");
		}

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
			case 'D': case 'E': // can only run next activity/echo if the current one has finished
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
		rlogid(message->payload);

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
				__activity_duration_limit_warning = true;
				perform_do_command(message->payload);
				break;
			case 'E': // echo
				rlogfd("Echoing");
				robot::send_message('s', message->payload);
				break;
			case 'K': // config-key
				robot::configuration::set_config_key(message->payload);
				break;
			case 'S': // config-set
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
				robot::disable_distance_sensors();
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
					rlogf("Activity lookup failed");
					rlogi(payload);
					break;
				}

				start_activity(next_activity);

				break;
		}
	}

}
