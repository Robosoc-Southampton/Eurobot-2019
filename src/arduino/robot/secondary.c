
#include "include/robot/secondary.h"

#include <Stepper.h>

Stepper raisingStepper(200, 8, 9, 10, 11);

ACTIVITY(raiseStepper, cooldown=1000, count=1000) {
	raisingStepper.step(5);
}

ACTIVITY(lowerStepper, cooldown=1000, count=1000) {
	raisingStepper.step(-5);
}

int16_t readComponentValue(int16_t component_ID) {
	return 0u;
}

struct Activity* lookupActivity(uint16_t activity_ID) {
	if (activity_ID == 1) {
		return ACTIVITY(raiseStepper);
	}
	if (activity_ID == 2) {
		return ACTIVITY(lowerStepper);
	}

	return nullptr;
}

SharpIR sensors[] = {};

void setup() {
	Serial.begin(9600);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(0, sensors);

	robot::setup();

	rlogf("Setting up raising stepper speed");
	raisingStepper.setSpeed(200);
}

void loop() {
	robot::loop();
}
