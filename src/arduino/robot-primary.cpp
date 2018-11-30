
#include "robot-primary.h"

MD25 md25;

auto ledOn = LOW;

ACTIVITY(toggleLED, 500000, 5000000) {
	digitalWrite(LED_BUILTIN, ledOn);
	ledOn ^= HIGH;
}

uint16_t readComponentValue(uint16_t component_ID) {
	return 0u;
}

Activity* lookupActivity(uint16_t activity_ID) {
	return &toggleLED;
}

Sensor *sensors;

void primary_setup() {
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

	md25.setup();

	sensors = malloc(1);

	toggleLED_setup();

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(0, sensors);
	robot::configuration::set_radii(50u, 150u);
	robot::drive::set_md25(&md25);

	robot::setup();
}

void primary_loop() {
	robot::loop();
}
