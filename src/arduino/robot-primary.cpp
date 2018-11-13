
#include "robot-primary.h"

MD25 md25;

void primary_setup() {
	Serial.begin(9600);

	md25.setup();

	Serial.print("Battery voltage: ");
	Serial.println(md25.getBatteryVoltage());

	delay(1000);

	md25.setLeftMotorSpeed(148);
	md25.setRightMotorSpeed(148);

	delay(1000);

	md25.setLeftMotorSpeed(108);
	md25.setRightMotorSpeed(108);

	delay(1000);

	md25.setLeftMotorSpeed(108);
	md25.setRightMotorSpeed(148);

	delay(2000);

	md25.setLeftMotorSpeed(148);
	md25.setRightMotorSpeed(108);
}

void primary_loop() {
	
}
