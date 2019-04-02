
#include "include/robot/primary.h"

/* LED blink activity
 *  Blinks the LED on and off 5 times
 */
auto ledOn = LOW;

ACTIVITY(blinkLED, cooldown=500000, count=10) {
	ledOn ^= HIGH;
	digitalWrite(LED_BUILTIN, ledOn);
}

INIT(blinkLED) {
	ledOn = LOW;
	digitalWrite(LED_BUILTIN, LOW);
}

/* Pull cord activity
 *  Waits until the pull cord has been pulled before returning
 */
uint8_t pullCordPin = 12;
bool pullCordInserted = false;

ACTIVITY(pullCord, cooldown=1000000, count=900) {
	if (!digitalRead(pullCordPin) && !pullCordInserted) {
		pullCordInserted = true;
		rlogf("Pull cord inserted");
	}
}

INIT(pullCord) {
	if (!pullCordInserted)
		rlogf("Waiting for pull cord to be inserted");
}

PREDICATE(pullCord) {
	if (pullCordInserted && digitalRead(pullCordPin)) {
		rlogf("Pull cord pulled");
		return false;
	}

	return true;
}

/* Component reader
 *  Allows component values to be read remotely
 */
int16_t readComponentValue(int16_t component_ID) {
	return robot::drive::get_average_distance_travelled();
}

/* Activity 5 blinks the LED 5 times
 * Activity 100 waits for the pull cord
 */
struct Activity* lookupActivity(uint16_t activity_ID) {
	switch (activity_ID) {
		case 5:
			return ACTIVITY(blinkLED);
		case 100: 
			return ACTIVITY(pullCord);
	}

	return nullptr;
}

SharpIR sensors[] = {
	SharpIR(A0, 606, 1202, 1112, 529, 124, 11)
};

void setup() {
	Serial.begin(9600);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(1, sensors);

	robot::setup();

	rlogf("Setting pull cord pin to input");
	pinMode(pullCordPin, INPUT);

	rlogf("Done");
}

void loop() {
	robot::loop();
}
