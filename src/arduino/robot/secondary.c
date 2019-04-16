
#include "include/robot/secondary.h"

auto ledOn = LOW;
Stepper raisingStepper(STEPPER_ROTATIONS, LIFTING_STEPPER_1, LIFTING_STEPPER_2, LIFTING_STEPPER_3, LIFTING_STEPPER_4);
Servo gripServo, finger;

DistanceSensor sensors[] = {
};

///////////////////////////////////////////////////

ACTIVITY(raiseStepper, cooldown=1650, count=3000) {
	raisingStepper.step(1);
}

///////////////////////////////////////////////////////

ACTIVITY(raiseStepperBlue, cooldown=1800, count=3000) {
	raisingStepper.step(1);
}

///////////////////////////////////////////////////

ACTIVITY(lowerStepper, cooldown=1500, count=3000) {
	raisingStepper.step(-1);
}

///////////////////////////////////////////////////////

ACTIVITY(raiseStepperSmall, cooldown=2000, count=400) {
	raisingStepper.step(1);
}

///////////////////////////////////////////////////////

ACTIVITY(lowerStepperSmall, cooldown=2000, count=400) {
	raisingStepper.step(-1);
}

///////////////////////////////////////////////

ACTIVITY(blinkLED, cooldown=500000, count=10) {
	ledOn ^= HIGH;
	digitalWrite(LED_BUILTIN, ledOn);
}

START(blinkLED) {
	ledOn = LOW;
	digitalWrite(LED_BUILTIN, LOW);
}

//////////////////////////////////////////

ACTIVITY(closeGrip, cooldown=0, count=0) {

}

START(closeGrip) {
	gripServo.write(35);
}

//////////////////////////////////////////

ACTIVITY(openGrip, cooldown=0, count=0) {

}

START(openGrip) {
	gripServo.write(0);
}

/////////////////////////////////////////////////

ACTIVITY(pullCord, cooldown=1000000, count=900) {
	/* do nothing */
}

START(pullCord) {
	rlogf("Waiting for pull cord to be inserted");
}

PREDICATE(pullCord) {
	return !robot::wait_for_pullcord(PULL_CORD_PIN);
}

//////////////////////////////////////////

ACTIVITY(openFinger, cooldown=0, count=0) {

}

START(openFinger) {
	finger.write(360);
}

//////////////////////////////////////////

ACTIVITY(closeFinger, cooldown=0, count=0) {

}

START(closeFinger) {
	finger.write(0);
}

//////////////////////////////////////////////////

int16_t readComponentValue(int16_t component_ID) {
	return 0u;
}

struct Activity* lookupActivity(uint16_t activity_ID) {
	switch (activity_ID) {
		case ACTIVITY_LOWER_STEPPER:
			return ACTIVITY(lowerStepper);
		case ACTIVITY_RAISE_STEPPER:
			return ACTIVITY(raiseStepper);
		case ACTIVITY_RAISE_STEPPER_BLUE:
			return ACTIVITY(raiseStepperBlue);
		case ACTIVITY_LOWER_STEPPER_SMALL:
			return ACTIVITY(lowerStepperSmall);
		case ACTIVITY_RAISE_STEPPER_SMALL:
			return ACTIVITY(raiseStepperSmall);
		case ACTIVITY_BLINK_LED:
			return ACTIVITY(blinkLED);
		case ACTIVITY_CLOSE_GRIP:
			return ACTIVITY(closeGrip);
		case ACTIVITY_OPEN_GRIP:
			return ACTIVITY(openGrip);
		case ACTIVITY_PULL_CORD:
			return ACTIVITY(pullCord);
		case ACTIVITY_OPEN_FINGER:
			return ACTIVITY(openFinger);
		case ACTIVITY_CLOSE_FINGER:
			return ACTIVITY(closeFinger);
	}

	return nullptr;
}

void setup() {
	Serial.begin(9600);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(0, sensors);

	robot::setup();

	rlogf("Setting up raising stepper speed");
	raisingStepper.setSpeed(STEPPER_SPEED);

	rlogf("Attaching grip servo");
	gripServo.attach(GRIP_SERVO_PIN);
	gripServo.write(0);

	rlogf("Attaching finger servo");
	finger.attach(FINGER_SERVO_PIN);
	finger.write(0);

	rlogf("Setting pull cord pin to input");
	pinMode(PULL_CORD_PIN, INPUT);

	rlogf("Done");
}

void loop() {
	robot::loop();
}
