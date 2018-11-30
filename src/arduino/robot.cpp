
#include "robot.h"

namespace robot {
	robot::Activity *current_activity = nullptr;
	uint32_t current_activity_cooldown = 0;
	uint32_t current_activity_clock = 0;
	uint32_t current_activity_last_call = 0;

	bool is_moving = false;

	robot::ReadComponentValue component_value_reader = nullptr;

	void setup() {
		rassert(component_value_reader != nullptr, "Component value reader has not been set");
	}

	void loop() {
		check_distance_sensors();
		update_motor_speeds();
		run_activity();
		read_message_buffer();
	}

	void check_distance_sensors() {
		
	}

	void update_motor_speeds() {

	}

	void run_activity() {
		if (current_activity == nullptr) return;

		uint32_t current_time = micros();
		uint32_t delta_time = current_time - current_activity_last_call;

		// update clock and cooldown
		current_activity_cooldown -= delta_time;
		current_activity_clock += delta_time;

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
		if (delta_time >= current_activity_cooldown) {
			// run the activity
			current_activity->callback();
			// note the last call time
			current_activity_last_call = current_time;
			// reset the cooldown (TODO: review this)
			current_activity_cooldown = delta_time >= current_activity->cooldown
			                          ? current_activity->cooldown
			                          : current_activity->cooldown - delta_time + current_activity_cooldown;
		}
	}

	void read_message_buffer() {
		// if no message is available, stop
		if (Serial.available() < MESSAGE_SIZE) return;

		bool is_activity_running = current_activity != nullptr;
		bool is_readable = false;
		Message message;

		// test the command char to determine if the message is ready to be read
		switch ((char) Serial.peek()) {
			case 'F': case 'T': case 'A': // forward, turn and align commands
				// can only run next movement command if not currently moving
				is_readable = !is_moving;
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
		if (!is_readable) return;

		// read message from serial buffer
		uint8_t bytes[MESSAGE_SIZE];

		for (uint8_t i = 0; i < MESSAGE_SIZE; ++i) {
			bytes[i] = Serial.read();
		}

		message = Message(bytes);

		// act on message
		switch (message.command) {
			case 'F': // forward
				// TODO: implement forward
				break;
			case 'T': // turn
				// TODO: implement turn
				break;
			case 'A': // align
				// TODO: implement align
				break;
			case 'D': // do
				// TODO: implement do
				break;
			case 'R': // request
				uint16_t value = (*component_value_reader)(message.payload);
				send_message('r', value);
				break;
		}
	}

	void set_component_value_reader(ReadComponentValue reader) {
		component_value_reader = reader;
	}

	void send_message(char command, uint16_t payload) {
		Serial.write(command);
		Serial.write((uint8_t*) &payload, 2); // this is beautifully hacky right?
	}
}
