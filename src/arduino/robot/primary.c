
#include "include/robot/primary.h"

auto ledOn = LOW;
Stepper carouselStepper(STEPPER_ROTATIONS, CAROUSEL_STEPPER_1, CAROUSEL_STEPPER_2, CAROUSEL_STEPPER_3, CAROUSEL_STEPPER_4);
Stepper armStepper(STEPPER_ROTATIONS, ARM_STEPPER_1, ARM_STEPPER_2, ARM_STEPPER_3, ARM_STEPPER_4);
Servo primaryArmServo, secondaryArmServo, grabberServo, frontAlignmentServo1, frontAlignmentServo2;

DistanceSensor sensors[] = {
	UltraSonic(34, 42, 300),
	UltraSonic(36, 40, 200),
	UltraSonic(32, 44, 300)
};

///////////////////////////////////////////////

ACTIVITY(blinkLED, cooldown=500000, count=10) {
	ledOn ^= HIGH;
	digitalWrite(LED_BUILTIN, ledOn);
}

START(blinkLED) {
	ledOn = LOW;
	digitalWrite(LED_BUILTIN, LOW);
}

///////////////////////////////////////////////////

ACTIVITY(rotateCarousel, cooldown=4000, count=121) {
	carouselStepper.step(-1);
}

///////////////////////////////////////////////////////

ACTIVITY(rotateCarouselHalf, cooldown=1500, count=80) {
	carouselStepper.step(-1);
}

/////////////////////////////////////////////////////

ACTIVITY(rotateCarousel5, cooldown=4000, count=595) {
	carouselStepper.step(-1);
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

///////////////////////////////////////////////////////

ACTIVITY(pullCordInsert, cooldown=1000000, count=900) {
	/* do nothing */
}

START(pullCordInsert) {
	rlogf("Waiting for pull cord to be inserted");
}

PREDICATE(pullCordInsert) {
	return digitalRead(PULL_CORD_PIN);
}

///////////////////////////////////////////////////////

ACTIVITY(alignForward, cooldown=250000, count=8) {
	if (activity_iteration == 1) {
		robot::drive::md25->setLeftMotorSpeed(160);
		robot::drive::md25->setRightMotorSpeed(160);
	}

	if (activity_iteration == 3) {
		robot::drive::md25->setLeftMotorSpeed(120);
		robot::drive::md25->setRightMotorSpeed(120);
	}

	if (activity_iteration == 4) {
		robot::drive::md25->stopMotors();
	}

	if (activity_iteration == 6) {
		frontAlignmentServo1.write(72);
		frontAlignmentServo2.write(180);
	}
}

START(alignForward) {
	frontAlignmentServo1.write(150);
	frontAlignmentServo2.write(94);
}

STOP(alignForward) {
	robot::drive::md25->resetEncoders();
}

///////////////////////////////////////////////////////

ACTIVITY(alignBackward, cooldown=250000, count=5) {
	if (activity_iteration == 3) robot::drive::md25->stopMotors();
}

START(alignBackward) {
	robot::drive::md25->setLeftMotorSpeed(100);
	robot::drive::md25->setRightMotorSpeed(100);
}

STOP(alignBackward) {
	robot::drive::md25->resetEncoders();
}

/////////////////////////////////////////////////////

ACTIVITY(raiseArmSlightly, cooldown=1500, count=50) {
	armStepper.step(1);
}

/////////////////////////////////////////////////////

ACTIVITY(lowerArmSlightly, cooldown=1500, count=50) {
	armStepper.step(-1);
}

/////////////////////////////////////////////////////

ACTIVITY(armPositionPA, cooldown=1500, count=260) {
	if (activity_iteration >= 200) armStepper.step(-1);
}

START(armPositionPA) {
	primaryArmServo.write(60);
	secondaryArmServo.write(40);
	grabberServo.write(50);
}

/////////////////////////////////////////////////////

ACTIVITY(armPositionPARetract, cooldown=1500, count=60) {
	armStepper.step(1);
}

STOP(armPositionPARetract) {
	primaryArmServo.write(80);
	secondaryArmServo.write(30);
}

/////////////////////////////////////////////////////

ACTIVITY(armPositionGoldium, cooldown=1500, count=310) {
	if (activity_iteration < 100) return;
	if (activity_iteration < 210) armStepper.step(-1);
	if (activity_iteration == 160) grabberServo.write(ARM_GRABBER_OPEN);
}

START(armPositionGoldium) {
	primaryArmServo.write(30);
	secondaryArmServo.write(15);
}

STOP(armPositionGoldium) {
	primaryArmServo.write(45);
	secondaryArmServo.write(50);
}

/////////////////////////////////////////////////////

ACTIVITY(armPositionGoldiumRetract, cooldown=1500, count=260) {
	if (activity_iteration >= 150) armStepper.step(1);
	if (activity_iteration == 100) grabberServo.write(ARM_GRABBER_CLOSED);
}

/////////////////////////////////////////////////////

ACTIVITY(dropGoldium, cooldown=1500, count=0) {
	
}

START(dropGoldium) {
	grabberServo.write(ARM_GRABBER_OPEN);
}

/////////////////////////////////////////////////////

ACTIVITY(dropGoldiumRetract, cooldown=1500, count=0) {
	
}

START(dropGoldiumRetract) {
	primaryArmServo.write(80);
	secondaryArmServo.write(40);
}

////////////////////////////////////////////////////////

ACTIVITY(lowerFromTopToSide, cooldown=2000, count=400) {
	if (activity_iteration >= 100) armStepper.step(-1);
	if (activity_iteration == 350) secondaryArmServo.write(38);
}

START(lowerFromTopToSide) {
	primaryArmServo.write(50);
	secondaryArmServo.write(45);
	grabberServo.write(ARM_GRABBER_OPEN);
}

///////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToTopRed, cooldown=2000, count=550) {
	if (activity_iteration >= 100 && activity_iteration < 400) armStepper.step(1);
	if (activity_iteration == 400) secondaryArmServo.write(60);
	if (activity_iteration == 500) {
		primaryArmServo.write(83);
		secondaryArmServo.write(15);
	}
}

START(raiseFromSideToTopRed) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

///////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToRest, cooldown=2000, count=550) {
	if (activity_iteration >= 100 && activity_iteration < 400) armStepper.step(1);
	if (activity_iteration == 400) secondaryArmServo.write(60);
	if (activity_iteration == 500) {
		primaryArmServo.write(82);
		secondaryArmServo.write(15);
	}
}

/////////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToTopGreen, cooldown=2000, count=550) {
	if (activity_iteration >= 100 && activity_iteration < 400) armStepper.step(1);
	if (activity_iteration == 400) secondaryArmServo.write(60);
	if (activity_iteration == 500) {
		primaryArmServo.write(82);
		secondaryArmServo.write(15);
	}
}

START(raiseFromSideToTopGreen) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

////////////////////////////////////////////////////////////

ACTIVITY(raiseFromSideToTopBlue, cooldown=2000, count=550) {
	if (activity_iteration >= 100 && activity_iteration < 400) armStepper.step(1);
	if (activity_iteration == 400) secondaryArmServo.write(60);
	if (activity_iteration == 500) {
		primaryArmServo.write(82);
		secondaryArmServo.write(15);
	}
}

START(raiseFromSideToTopBlue) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

//////////////////////////////////////////////////////

ACTIVITY(lowerIntoCarousel, cooldown=1500, count=300) {
	if (activity_iteration >= 100 && activity_iteration < 250) armStepper.step(-1);
	if (activity_iteration == 250) primaryArmServo.write(81);
}

START(lowerIntoCarousel) {
	
}

STOP(lowerIntoCarousel) {
	grabberServo.write(ARM_GRABBER_OPEN);
	primaryArmServo.write(78);
}

//////////////////////////////////////////////////////

ACTIVITY(raiseFromCarousel, cooldown=1500, count=250) {
	if (activity_iteration == 50) primaryArmServo.write(85);
	if (activity_iteration >= 100) armStepper.step(1);
}

STOP(raiseFromCarousel) {
	secondaryArmServo.write(5);
}

/////////////////////////////////////////////////////////

ACTIVITY(putInWeighingScales, cooldown=1500, count=400) {
	if (activity_iteration >= 200) armStepper.step(-1);
}

START(putInWeighingScales) {
	primaryArmServo.write(20);
}

STOP(putInWeighingScales) {
	secondaryArmServo.write(50);
	grabberServo.write(ARM_GRABBER_OPEN);
}

///////////////////////////////////////////////////////////////

ACTIVITY(retractFromWeighingScales, cooldown=1500, count=500) {
	if (activity_iteration < 200) armStepper.step(1);
	if (activity_iteration == 100) secondaryArmServo.write(5);
	if (activity_iteration == 300) primaryArmServo.write(90);
}

START(retractFromWeighingScales) {
	secondaryArmServo.write(60);
}

//////////////////////////////////////////////////////

ACTIVITY(pickIntoCarousel, cooldown=1500, count=330) {
	if (activity_iteration >= 200) armStepper.step(-1);
}

START(pickIntoCarousel) {
	primaryArmServo.write(83);
	secondaryArmServo.write(25);
	grabberServo.write(ARM_GRABBER_OPEN);
}

//////////////////////////////////////////////////////

ACTIVITY(pickFromCarousel, cooldown=1500, count=180) {
	if (activity_iteration >= 50) armStepper.step(1);
}

START(pickFromCarousel) {
	grabberServo.write(ARM_GRABBER_CLOSED);
}

//////////////////////////////////////////////////////////////

ACTIVITY(putOnParticleAccelerator, cooldown=1500, count=470) {
	if (activity_iteration >= 200) armStepper.step(-1);
	if (activity_iteration == 200) secondaryArmServo.write(55);
}

START(putOnParticleAccelerator) {
	primaryArmServo.write(20);
}

STOP(putOnParticleAccelerator) {
	grabberServo.write(ARM_GRABBER_OPEN);
}

/////////////////////////////////////////////////////////////////

ACTIVITY(pullBackParticleAccelerator, cooldown=1500, count=270) {
	armStepper.step(1);
}

START(pullBackParticleAccelerator) {
	secondaryArmServo.write(10);
}

STOP(pullBackParticleAccelerator) {
	primaryArmServo.write(85);
}

/////////////////////////////////////////////////

ACTIVITY(allowRotate, cooldown=1500, count=15) {
	armStepper.step(1);
}

START(allowRotate) {
	primaryArmServo.write(85);
	secondaryArmServo.write(15);
}

///////////////////////////////////////////////////

ACTIVITY(unAllowRotate, cooldown=1500, count=15) {
	armStepper.step(-1);
}

START(unAllowRotate) {
	primaryArmServo.write(85);
	secondaryArmServo.write(25);
}

////////////////////////////////////////////

ACTIVITY(fastArm, cooldown=1500, count=0) {
	
}

START(fastArm) {
	armStepper.setSpeed(ARM_STEPPER_SPEED_FAST);
}

////////////////////////////////////////////

ACTIVITY(slowArm, cooldown=1500, count=0) {
	
}

START(slowArm) {
	armStepper.setSpeed(ARM_STEPPER_SPEED_SLOW);
}

//////////////////////////////////////////////////

int16_t readComponentValue(int16_t component_ID) {
	return robot::drive::get_average_distance_travelled();
}

struct Activity* lookupActivity(uint16_t activity_ID) {
	switch (activity_ID) {
		case ACTIVITY_ROTATE_CAROUSEL:
			return ACTIVITY(rotateCarousel);
		case ACTIVITY_ROTATE_CAROUSEL_HALF:
			return ACTIVITY(rotateCarouselHalf);
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
		case ACTIVITY_PUT_IN_WEIGHING_SCALES:
			return ACTIVITY(putInWeighingScales);
		case ACTIVITY_RETRACT_FROM_WEIGHING_SCALES:
			return ACTIVITY(retractFromWeighingScales);
		case ACTIVITY_PICK_INTO_CAROUSEL:
			return ACTIVITY(pickIntoCarousel);
		case ACTIVITY_PICK_FROM_CAROUSEL:
			return ACTIVITY(pickFromCarousel);
		case ACTIVITY_PUT_ON_PARTICLE_ACCELERATOR:
			return ACTIVITY(putOnParticleAccelerator);
		case ACTIVITY_PULL_BACK_PARTICLE_ACCELERATOR:
			return ACTIVITY(pullBackParticleAccelerator);
		case ACTIVITY_ALLOW_ROTATE:
			return ACTIVITY(allowRotate);
		case ACTIVITY_UN_ALLOW_ROTATE:
			return ACTIVITY(unAllowRotate);
		case ACTIVITY_PULL_CORD:
			return ACTIVITY(pullCord);
		case ACTIVITY_PULL_CORD_INSERT:
			return ACTIVITY(pullCordInsert);
		case ACTIVITY_RAISE_ARM_FROM_SIDE_TO_REST:
			return ACTIVITY(raiseFromSideToRest);
		case ACTIVITY_ROTATE_CAROUSEL_5:
			return ACTIVITY(rotateCarousel5);
		case ACTIVITY_FAST_ARM:
			return ACTIVITY(fastArm);
		case ACTIVITY_SLOW_ARM:
			return ACTIVITY(slowArm);
		case ACTIVITY_ALIGN_FORWARD:
			return ACTIVITY(alignForward);
		case ACTIVITY_ALIGN_BACKWARD:
			return ACTIVITY(alignBackward);
		case ACTIVITY_ARM_POSITION_PA:
			return ACTIVITY(armPositionPA);
		case ACTIVITY_ARM_POSITION_PA_RETRACT:
			return ACTIVITY(armPositionPARetract);
		case ACTIVITY_ARM_POSITION_GOLDIUM:
			return ACTIVITY(armPositionGoldium);
		case ACTIVITY_ARM_POSITION_GOLDIUM_RETRACT:
			return ACTIVITY(armPositionGoldiumRetract);
		case ACTIVITY_DROP_GOLDIUM:
			return ACTIVITY(dropGoldium);
		case ACTIVITY_DROP_GOLDIUM_RETRACT:
			return ACTIVITY(dropGoldiumRetract);
	}

	return nullptr;
}

void setup() {
	Serial.begin(9600);

	pinMode(LED_GROUND_PIN, OUTPUT);
	digitalWrite(LED_GROUND_PIN, LOW);

	primaryArmServo.attach(PRIMARY_SERVO_PIN);
	primaryArmServo.write(0);

	robot::set_component_value_reader(&readComponentValue);
	robot::set_activity_lookup(&lookupActivity);
	robot::set_distance_sensors(3, sensors);

	robot::setup();

	rlogf("Setting initial arm stepper speed");
	armStepper.setSpeed(50);

	rlogf("Setting carousel stepper speed");
	carouselStepper.setSpeed(CAROUSEL_STEPPER_SPEED);

	rlogf("Attaching arm servos");
	secondaryArmServo.attach(SECONDARY_SERVO_PIN);
	grabberServo.attach(GRABBER_SERVO_PIN);

	rlogf("Attaching alignment servos");
	// TODO
	frontAlignmentServo1.attach(FRONT_ALIGNMENT_SERVO_1_PIN);
	frontAlignmentServo2.attach(FRONT_ALIGNMENT_SERVO_2_PIN);
	frontAlignmentServo1.write(75);
	frontAlignmentServo2.write(180);

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
	armStepper.step(170);

	rlogf("Setting arm stepper speed");
	armStepper.setSpeed(ARM_STEPPER_SPEED_FAST);

	rlogf("Done");
}

void loop() {
	robot::loop();
}
