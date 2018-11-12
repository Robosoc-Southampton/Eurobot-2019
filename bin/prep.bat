
@echo off

echo // auto-generated > src/arduino/robot.ino
echo #include "robot-primary.h" >> src/arduino/robot.ino
echo #include "robot-secondary.h" >> src/arduino/robot.ino
echo void setup() { %1_setup(); } >> src/arduino/robot.ino
echo void  loop() {  %1_loop(); } >> src/arduino/robot.ino
