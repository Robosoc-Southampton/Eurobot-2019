
#include "include/robot/test/sharpIR.h"

SharpIR sensor = SharpIR(A0);

void setup() {
	Serial.begin(9600);
	
	robot::wait_for_connection();

	rlog("Sensor connected to A0");
}

void loop() {
	rlogi(sensor.read());

	delay(200);
}
