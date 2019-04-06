
#include "include/robot/test/sharpIR.h"

SharpIR sensor = SharpIR(A0, 606, 1202, 1112, 529, 124, 11);
bool show_reading = false;
bool show_voltage = true;
int16_t delay_duration = 500;

void setup() {
	Serial.begin(9600);
	
	robot::wait_for_connection();

	rlog("Sensor connected to A0");
}

void loop() {
	robot::update_message_buffer();

	if (robot::peek_next_opcode() != '\0') {
		Message *message = robot::read_message_buffer();

		if (message->opcode == 'K') {
			switch (message->payload) {
				case 0: show_reading = false;
				case 1: show_reading = true;
				case 2: show_voltage = false;
				case 3: show_voltage = true;
			}
		}
		if (message->opcode == 'S') {
			delay_duration = message->payload;
		}
	}

	if (show_reading) {
		rlog("Sensor reading:");
		rlogi(sensor.read());
	}

	if (show_voltage) {
		rlog("Sensor voltage (/1024):"); 
		rlogi(analogRead(A0));
	}

	delay(delay_duration);
}
