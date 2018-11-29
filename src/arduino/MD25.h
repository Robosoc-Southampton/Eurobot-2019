
#pragma once

#include <arduino.h>
#include <Wire.h>
#include "debug.h"

class MD25 {
public:
	// creates the MD25 object
	MD25();

	// sets up the MD25 prior to calling any further functions
	void setup();

	// sets and gets the acceleration of the MD25
	// valid acceleration values are in the range 1 to 10
	// see https://www.robot-electronics.co.uk/htm/md25i2c.htm#acceleration%20register
	void setAcceleration(uint8_t acceleration);
	uint8_t getAcceleration();

	// sets the left/right motor speeds
	void setLeftMotorSpeed(uint8_t speed);
	void setRightMotorSpeed(uint8_t speed);
	void stopMotors();

	// resets the encoders
	void resetEncoders();
	// reads the value of the left/right encoders
	int32_t readLeftEncoder();
	int32_t readRightEncoder();

	// returns the battery voltage multiplied by 10 (e.g. 12.1V -> 121)
	uint8_t getBatteryVoltage();

private:
	static const uint8_t MD25_ADDRESS = 0x58u,
	                     SPEED1_REGISTER = 0x00u,
	                     SPEED2_REGISTER = 0x01u,
	                     ENCODER1_REGISTER = 0x02,
	                     ENCODER2_REGISTER = 0x06,
	                     VOLTAGE_REGISTER = 0x0A,
	                     ENCODER_RESET = 0x20,
	                     MODE_SELECTOR = 0x0F,
	                     ACCELERATION_REGISTER = 0x0E,
	                     COMMAND_REGISTER = 0x10;
	static const uint8_t STOP_SPEED = 128,
	                     DEFAULT_ACCELERATION = 5;

	static inline __attribute__((always_inline)) void i2c_write(uint8_t register, uint8_t data);
	static inline __attribute__((always_inline)) int32_t i2c_read4(uint8_t register);
	static inline __attribute__((always_inline)) uint8_t i2c_read1(uint8_t register);

	static const char* MD25_NOT_SETUP;

	uint8_t acceleration;
	bool isSetup;

};
