
#include "include/robot/secondary.h"

#include <Stepper.h>

/* Blink LED activity
 *  Blinks the LED 10 times
 */
auto ledOn = LOW;

ACTIVITY(blinkLED, cooldown=500000, count=10) {
	ledOn ^= HIGH;
	digitalWrite(LED_BUILTIN, ledOn);
}

INIT(blinkLED) {
	ledOn = LOW;
}

/* Stepper control activities
 *  Controls the front grabber elevation through 4 up/down activities
 */
Stepper raisingStepper(200, 8, 9, 10, 11);

ACTIVITY(raiseStepper, cooldown=3000, count=500) {
	raisingStepper.step(2);
}

ACTIVITY(lowerStepper, cooldown=3000, count=500) {
	raisingStepper.step(-2);
}

ACTIVITY(raiseStepperSmall, cooldown=3000, count=200) {
	raisingStepper.step(2);
}

ACTIVITY(lowerStepperSmall, cooldown=3000, count=200) {
	raisingStepper.step(-2);
}

/* Component reader
 *  Allows component values to be read remotely
 */
int16_t readComponentValue(int16_t component_ID) {
	return 0u;
}

/* Activity 1 lowers front grabber
 * Activity 2 raises front grabber
 * Activity 3 lowers front grabber slightly
 * Activity 4 raises front grabber slightly
 * Activity 5 blinks the LED 5 times
 */
struct Activity* lookupActivity(uint16_t activity_ID) {
	switch (activity_ID) {
		case 1:
			return ACTIVITY(lowerStepper);
		case 2:
			return ACTIVITY(raiseStepper);
		case 3:
			return ACTIVITY(lowerStepperSmall);
		case 4:
			return ACTIVITY(raiseStepperSmall);
		case 5:
			return ACTIVITY(blinkLED);
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
