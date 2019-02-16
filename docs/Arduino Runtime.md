
# Arduino Runtime

## High level details

The arduino will execute a loop doing the following tasks:

* check distance sensors
* update motor speeds
* run activity
* read message buffer

The arduino receives commands via [control interface](/docs/Control%20Interface.md) messages, which populate the message buffer, and affect the execution of the loop above.

Drive messages (e.g. `forward` and `turn`) will affect the [drive system](/src/arduino/drive.h), which in turn affects the motor speed targets.

Some component messages (e.g. `do`) will affect the running activity.

## Activities

> See [activities.h](/src/arduino/activities.h)

An activity has the following properties

* callback - function called every frame
* predicate? - function that must return `true` for the activity to continue
* cooldown - time (in microseconds) between frames
* timeout - time to wait (in microseconds) before stopping activity
	* a timeout of 0 represents no timeout (i.e. runs until predicate fails)
