
#include "include/robot/test/sharpIR.h"

SharpIR sensor = SharpIR(A0, 606, 1202, 1112, 529, 124, 11);

void setup() {
	Serial.begin(9600);
	
	robot::wait_for_connection();

	rlog("Sensor connected to A0");
}

void loop() {
	// rlog("Sensor reading");
	// rlogi(sensor.read());
	rlog("Sensor voltage (/1024)");
	rlogi(analogRead(A0));

	delay(500);
}
