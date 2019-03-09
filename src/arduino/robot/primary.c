
#include "include/robot/primary.h"

auto ledOn = LOW;

ACTIVITY(toggleLED, cooldown=500000, count=10) {
	ledOn ^= HIGH;
	digitalWrite(LED_BUILTIN, ledOn);
}

INIT(toggleLED) {
	ledOn = LOW;
	digitalWrite(LED_BUILTIN, LOW);
}

int16_t readComponentValue(int16_t component_ID) {
	return robot::drive::get_average_distance_travelled();
}

struct Activity* lookupActivity(uint16_t activity_ID) {
	return ACTIVITY(toggleLED);
}

SharpIR sensors[] = {
	SharpIR(A0, 606, 1202, 1112, 529, 124, 11)
};

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(1, sensors);

	robot::setup();
}

void loop() {
	robot::loop();
}
