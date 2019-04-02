
#include "include/robot/secondary.h"

#include <Stepper.h>
#include <Servo.h>

/* Stepper control activities
 *  Controls the front grabber elevation through 4 up/down activities
 */
Stepper raisingStepper(200, 8, 9, 10, 11);

ACTIVITY(raiseStepper, cooldown=3000, count=1000) {
	raisingStepper.step(2);
}

ACTIVITY(lowerStepper, cooldown=3000, count=1000) {
	raisingStepper.step(-2);
}

ACTIVITY(raiseStepperSmall, cooldown=3000, count=200) {
	raisingStepper.step(2);
}

ACTIVITY(lowerStepperSmall, cooldown=3000, count=200) {
	raisingStepper.step(-2);
}

/* Servo control activities
 *  Controls the front grabber grip servo
 */
Servo gripServo;

ACTIVITY(closeGrip, cooldown=0, count=0) {}
ACTIVITY(openGrip, cooldown=0, count=0) {}

INIT(closeGrip) {
	gripServo.write(35);
}

INIT(openGrip) {
	gripServo.write(0);
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
	return 0u;
}

/* Activity 1 lowers front grabber
 * Activity 2 raises front grabber
 * Activity 3 lowers front grabber slightly
 * Activity 4 raises front grabber slightly
 * Activity 5 blinks the LED 5 times
 * Activity 6 closes the grabber grip
 * Activity 7 opens the grabber grip
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
		// case 5:
			// return ACTIVITY(blinkLED);
		case 6:
			return ACTIVITY(closeGrip);
		case 7:
			return ACTIVITY(openGrip);
		case 100:
			return ACTIVITY(pullCord);
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

	rlogf("Attaching servo");
	gripServo.attach(5);
	gripServo.write(0);

	rlogf("Setting pull cord pin to input");
	pinMode(pullCordPin, INPUT);

	rlogf("Done");
}

void loop() {
	robot::loop();
}
