
MD25 md25;

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	// robot::wait_for_connection();

	md25.setup();

	rlog("MD25 set up");
}

void loop() {
	md25.setLeftMotorSpeed(200);
	md25.setRightMotorSpeed(200);
}
