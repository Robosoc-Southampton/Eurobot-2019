
# Control interface

This document defines the structure and parameters of messages sent between the Arduino microcontroller on either robot, and the Raspberry Pi on the secondary robot which handles high level logic and control

### Special tasks

Task name | Task ID range | Description
-|-|-
Nothing | `0` | Does nothing
Enable all distance sensors | `1000` | Enables all distance sensors
Disable all distance sensors | `1100` | Disables all distance sensors
Enable distance sensor | `1001` - `1100` | Enables distance sensor `t - 1000`, i.e. `1005` enables distance sensor 5.
Disable distance sensor | `1101` - `1200` | Disables distance sensor `t - 1100`, i.e. `1105` disables distance sensor 5.

### Special readings

Reading name | Reading ID range | Description
-|-|-
Distance sensor reading | `1001` - `1100` | Reads a distance sensor value

### Message communication

To facilitate messages being passed between robots, the `message` message exists.

When the Arduino/controller start up, they perform a handshake, with the Arduino sending `message 0` (in `robot::wait_for_connection()`) and the controller responding `message 1`.
This synchronises both robots, ensuring they're both reading to send/receive further messages.

After this, further `message` messages will aid controlling the robots.

Mode | Target | Description
-|-
`2` | Arduino | The start of a message batch to be read into the Arduino's message buffer.
`3` | Arduino | A break in the message batch (more messages to be read)
`4` | Arduino | The end of a message batch (no more messages to be read)
`5` | Controller | Request for more messages (response to mode `3`)
`6` | Controller | Buffer overflow occurred, last batch failed
`7` | Arduino | Reset the message buffer

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
request | R | component | Reads a value from a component (e.g. servo rotation). `component` is a number representing that component.
message | M | mode | Message buffer related messages, `mode` will be one of [`1` for connection established]
config-key | K | key | Sets the config key to write to next
config-set | S | value | Sets the configuration value given by the current config key

### Arduino to Pi

Name | Char | Parameter | Description
-|-|-|-
collision | c | distance | Fired during `forward`/`align` action by the arduino, sending distance travelled.
status | s | error | Returns an error code for some arbitrary action. If `error` is `0`, no error was encountered (success).
return | r | data | Response to a `request` message.
log | l | length | Notifies the pi that the arduino is about to send a debug message. `length` is the length of the message.
message | M | mode | Message buffer related messages, `mode` will be one of [`1` for connection established]
