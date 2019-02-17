
void setup() {
	Serial.begin(9600);
}

void loop() {
	if (Serial.available()) {
		Serial.write('l');
		Serial.write(0);
		Serial.write(1);
		Serial.write(Serial.read());
	}
}
