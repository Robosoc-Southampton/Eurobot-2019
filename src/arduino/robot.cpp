
#include "robot.h"

Message::Message(uint8_t bytes[]) {
	command = (char) bytes[0];
	payload = ((uint16_t) bytes[1] << 8) + (uint16_t) bytes[2];
}

Message::Message() { /* do nothing */ }

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
		read_message_buffer();
	}

	void read_message_buffer() {
		// if no message is available, stop
		if (Serial.available() < MESSAGE_SIZE) return;

		bool is_activity_running = current_activity != nullptr;
		bool is_readable = false;
		Message message;
		Activity *next_activity;

		// test the command char to determine if the message is ready to be read
		switch ((char) Serial.peek()) {
			case 'F': case 'T': case 'A': // forward, turn and align commands
				// can only run next movement command if not currently moving
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
				drive::forward(message.payload);
				break;
			case 'T': // turn
				drive::turn(message.payload);
				break;
			case 'A': // align
				drive::forward(-drive::ALIGN_DISTANCE);
				// TODO: spec requires to drive forward by message.payload after aligning
				break;
			case 'D': // do
				next_activity = (*lookup_activity)(message.payload);

				if (next_activity == nullptr) {
					// TODO: what to do here? error?
					break;
				}

				start_activity(next_activity);

				break;
			case 'R': // request
				uint16_t value = (*component_value_reader)(message.payload);
				send_message('r', value);
				break;
		}
	}

	void send_message(char command, uint16_t payload) {
		Serial.write(command);
		Serial.write((uint8_t*) &payload, 2); // this is beautifully hacky right?
	}

}
