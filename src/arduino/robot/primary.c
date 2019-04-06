
#include "include/robot/primary.h"

auto ledOn = LOW;
Stepper carouselStepper(STEPPER_ROTATIONS, CAROUSEL_STEPPER_1, CAROUSEL_STEPPER_2, CAROUSEL_STEPPER_3, CAROUSEL_STEPPER_4);
Stepper armStepper(STEPPER_ROTATIONS, ARM_STEPPER_1, ARM_STEPPER_2, ARM_STEPPER_3, ARM_STEPPER_4);
Servo primaryArmServo, secondaryArmServo, grabberServo;

SharpIR sensors[] = {
	SharpIR(A0, 220)
};

///////////////////////////////////////////////

ACTIVITY(blinkLED, cooldown=500000, count=10) {
	ledOn ^= HIGH;
	digitalWrite(LED_BUILTIN, ledOn);
}

INIT(blinkLED) {
	ledOn = LOW;
	digitalWrite(LED_BUILTIN, LOW);
}

/////////////////////////////////////////////////

ACTIVITY(pullCord, cooldown=1000000, count=900) {
	/* do nothing */
}

INIT(pullCord) {
	rlogf("Waiting for pull cord to be inserted");
}

PREDICATE(pullCord) {
	return !robot::wait_for_pullcord(PULL_CORD_PIN);
}

/////////////////////////////////////////////////////

ACTIVITY(raiseArmSlightly, cooldown=3000, count=25) {
	armStepper.step(-2);
}

/////////////////////////////////////////////////////

ACTIVITY(lowerArmSlightly, cooldown=3000, count=25) {
	armStepper.step(2);
}

////////////////////////////////////////////////////////

ACTIVITY(lowerFromTopToSide, cooldown=3000, count=175) {
	armStepper.step(-2);
}

INIT(lowerFromTopToSide) {
	primaryArmServo.write(50);
	secondaryArmServo.write(35);
	grabberServo.write(ARM_GRABBER_OPEN);
}

///////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToTopRed, cooldown=3000, count=175) {
	armStepper.step(2);
}

INIT(raiseFromSideToTopRed) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

STOP(raiseFromSideToTopRed) {
	secondaryArmServo.write(20);
	primaryArmServo.write(86);
}

/////////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToTopGreen, cooldown=3000, count=175) {
	armStepper.step(2);
}

INIT(raiseFromSideToTopGreen) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

STOP(raiseFromSideToTopGreen) {
	secondaryArmServo.write(20);
	primaryArmServo.write(85);
}

////////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToTopBlue, cooldown=3000, count=175) {
	armStepper.step(2);
}

INIT(raiseFromSideToTopBlue) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

STOP(raiseFromSideToTopBlue) {
	secondaryArmServo.write(20);
	primaryArmServo.write(83);
}

//////////////////////////////////////////////////////

ACTIVITY(lowerIntoCarousel, cooldown=3000, count=75) {
	armStepper.step(-2);
}

STOP(lowerIntoCarousel) {
	grabberServo.write(ARM_GRABBER_OPEN);
}

//////////////////////////////////////////////////////

ACTIVITY(raiseFromCarousel, cooldown=3000, count=75) {
	armStepper.step(2);
}

///////////////////////////////////////////////////

ACTIVITY(rotateCarousel, cooldown=3000, count=61) {
	carouselStepper.step(-2);
}

INIT(rotateCarousel) {
	carouselStepper.step(-1);
}

//////////////////////////////////////////////////

int16_t readComponentValue(int16_t component_ID) {
	return robot::drive::get_average_distance_travelled();
}

struct Activity* lookupActivity(uint16_t activity_ID) {
	switch (activity_ID) {
		case ACTIVITY_ROTATE_CAROUSEL:
			return ACTIVITY(rotateCarousel);
		case ACTIVITY_LOWER_ARM:
			return ACTIVITY(lowerArmSlightly);
		case ACTIVITY_RAISE_ARM:
			return ACTIVITY(raiseArmSlightly);
		case ACTIVITY_BLINK_LED:
			return ACTIVITY(blinkLED);
		case ACTIVITY_LOWER_ARM_FROM_TOP_TO_SIDE:
			return ACTIVITY(lowerFromTopToSide);
		case ACTIVITY_RAISE_ARM_FROM_SIDE_TO_TOP_RED:
			return ACTIVITY(raiseFromSideToTopRed);
		case ACTIVITY_RAISE_ARM_FROM_SIDE_TO_TOP_GREEN:
			return ACTIVITY(raiseFromSideToTopGreen);
		case ACTIVITY_RAISE_ARM_FROM_SIDE_TO_TOP_BLUE:
			return ACTIVITY(raiseFromSideToTopBlue);
		case ACTIVITY_LOWER_ARM_INTO_CAROUSEL:
			return ACTIVITY(lowerIntoCarousel);
		case ACTIVITY_RAISE_ARM_FROM_CAROUSEL:
			return ACTIVITY(raiseFromCarousel);
		case ACTIVITY_PULL_CORD: 
			return ACTIVITY(pullCord);
	}

	return nullptr;
}

void setup() {
	Serial.begin(9600);

	primaryArmServo.attach(PRIMARY_SERVO_PIN);
	primaryArmServo.write(0);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(1, sensors);

	robot::setup();

	rlogf("Setting up arm stepper speed");
	armStepper.setSpeed(STEPPER_SPEED);

	rlogf("Setting up carousel stepper speed");
	carouselStepper.setSpeed(STEPPER_SPEED);

	rlogf("Attaching arm servos");
	secondaryArmServo.attach(SECONDARY_SERVO_PIN);
	grabberServo.attach(GRABBER_SERVO_PIN);

	rlogf("Setting pull cord pin to input");
	pinMode(PULL_CORD_PIN, INPUT);

	rlogf("Setting initial arm position");
	secondaryArmServo.write(20);
	grabberServo.write(48);
	primaryArmServo.write(0);
	armStepper.step(350);
	primaryArmServo.write(50);
	armStepper.step(100);
	primaryArmServo.write(90);
	armStepper.step(130);

	rlogf("Done");
}

void loop() {
	robot::loop();
}
