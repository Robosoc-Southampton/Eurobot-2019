
#include "include/component/MD25.h"

MD25::MD25() {
	acceleration = DEFAULT_ACCELERATION;
	isSetup = false;
}

void MD25::setup() {
	Wire.begin();

	i2c_write(MODE_SELECTOR, 0);
	i2c_write(ACCELERATION_REGISTER, acceleration);

	isSetup = true;

	resetEncoders();
}

void MD25::testEncoders(bool *left, bool *right) {
	*left = i2c_test_read4(ENCODER1_REGISTER);
	*right = i2c_test_read4(ENCODER2_REGISTER);
}

void MD25::setAcceleration(uint8_t acceleration) {
	rassert(isSetup, MD25_NOT_SETUP);
	i2c_write(ACCELERATION_REGISTER, acceleration);
	this->acceleration = acceleration;
}

uint8_t MD25::getAcceleration() {
	return acceleration;
}

void MD25::setLeftMotorSpeed(uint8_t speed) {
	rassert(isSetup, MD25_NOT_SETUP);
	i2c_write(SPEED1_REGISTER, speed);
}

void MD25::setRightMotorSpeed(uint8_t speed) {
	rassert(isSetup, MD25_NOT_SETUP);
	i2c_write(SPEED2_REGISTER, speed);
}

void MD25::stopMotors() {
	rassert(isSetup, MD25_NOT_SETUP);
	i2c_write(SPEED1_REGISTER, STOP_SPEED);
	i2c_write(SPEED2_REGISTER, STOP_SPEED);
}

void MD25::resetEncoders() {
	rassert(isSetup, MD25_NOT_SETUP);
	i2c_write(COMMAND_REGISTER, ENCODER_RESET);
}

int32_t MD25::readLeftEncoder() {
	rassert(isSetup, MD25_NOT_SETUP);
	return i2c_read4(ENCODER1_REGISTER);
}

int32_t MD25::readRightEncoder() {
	rassert(isSetup, MD25_NOT_SETUP);
	return i2c_read4(ENCODER2_REGISTER);
}

uint8_t MD25::getBatteryVoltage() {
	return i2c_read1(VOLTAGE_REGISTER);
}

void MD25::i2c_write(uint8_t reg, uint8_t data) {
	Wire.beginTransmission(MD25_ADDRESS);
	Wire.write(reg);
	Wire.write(data);
	Wire.endTransmission();
}

int32_t MD25::i2c_read4(uint8_t reg) {
	int32_t value = 0;

	Wire.beginTransmission(MD25_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.requestFrom(MD25_ADDRESS, 4u); // request 4 bytes

	while(Wire.available() < 4); // wait for 4 bytes

	for (uint8_t i = 0; i < 4; ++i) {
		value <<= 8;
		value += Wire.read();
	}

	return value;
}

bool MD25::i2c_test_read4(uint8_t reg) {
	uint8_t to_read = 4;
	auto start_time = millis();

	Wire.beginTransmission(MD25_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.requestFrom(MD25_ADDRESS, 4u); // request 4 bytes

	while (Wire.available() < 4 && millis() - start_time < 10000); // wait for 4 bytes

	for (int i = 0; Wire.available() && to_read; ++i) {
		Wire.read();
		--to_read;
	}

	return to_read == 0;
}

uint8_t MD25::i2c_read1(uint8_t reg) {
	Wire.beginTransmission(MD25_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.requestFrom(MD25_ADDRESS, 1u); // request 4 bytes

	while(Wire.available() < 1); // wait for 4 bytes

	return Wire.read();
}
