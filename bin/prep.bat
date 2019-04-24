
@echo off

more "src/arduino/robot/%1.c" > src/arduino/robot.ino
more "src/arduino/include/robot/%1_config.h" > src/arduino/include/config.h
