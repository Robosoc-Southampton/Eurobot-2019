
#include "include/robot/secondary.h"

int16_t readComponentValue(int16_t component_ID) {
	return 0u;
}

struct Activity* lookupActivity(uint16_t activity_ID) {
	return nullptr;
}

SharpIR sensors[] = {};

void setup() {
	Serial.begin(9600);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(0, sensors);

	robot::setup();
}

void loop() {
	robot::loop();
}
