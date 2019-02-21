
# Control interface

This document defines the structure and parameters of messages sent between the Arduino microcontroller on either robot, and the Raspberry Pi on the secondary robot which handles high level logic and control

### Message structure

```c++
struct Message {
	char command;
	int16_t payload;
}
```

### Pi to Arduino

Name | Char | Parameter | Description
-|-|-|-
forward | F | distance | Moves the robot forward by the distance in `mm`. Fires `collision`/`status` message on completion/failure. |
turn | T | angle | Fires `status(0)` message on completion |
do | D | task | Starts an activity on the arduino (e.g. moving a servo). `task` is a number representing that activity.
align | A | distance | Moves backwards into the wall to align, then forwards by `distance`, similar to `forward`.
request | R | component | Reads a value from a component (e.g. servo rotation). `component` is a number representing that component.
message | M | mode | Message buffer related messages, `mode` will be one of [`1` for connection established]
config-key | K | key | Sets the config key to write to next
config-set | S | value | Sets the configuration value given by the current config key

### Arduino to Pi

| Name | Char | Parameter | Description |
|-|-|-|-|
| collision | c | distance | Fired during `forward`/`align` action by the arduino, sending distance travelled. |
| status | s | error | Returns an error code for some arbitrary action. If `error` is `0`, no error was encountered (success). |
| return | r | data | Response to a `request` message. |
| log | l | length | Notifies the pi that the arduino is about to send a debug message. `length` is the length of the message. |
