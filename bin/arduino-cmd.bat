
@echo off

arduino_debug src/arduino/robot.ino --board arduino:avr:uno --pref build.path=./out %*
