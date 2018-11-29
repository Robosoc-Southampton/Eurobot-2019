
#include "robot-primary.h"

MD25 md25;

void primary_setup() {
	Serial.begin(9600);

	md25.setup();
}

void primary_loop() {
	
}
