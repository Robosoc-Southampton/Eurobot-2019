
# Troubleshooting

The aim of this document is to provide a list of potential/previously encountered errors, with fixes and explanations.

The document focusses on Arduino-sided errors, as controller errors can be much more easily debugged. However, as the controller will be running headless in the competition, there may be a few entries here for common issues.

> The term "controller" will be used to refer to the Pi or laptop sending commands to either Arduino.

Good luck!

## General tips

If something isn't working, something may have logged/not been logged to the controller.
If this can be checked (you can mimic the controller commands using a laptop), that'll help for sure.

Make sure that `DEBUG` is set to `true` in `/src/arduino/include/debug.h` - this'll enable even more debug messages.

If a particular component/feature isn't working, or the setup isn't completing, try running some tests (e.g. `bin/upload <port> test/md25`).

---

## Carousel stepper is vibrating but not rotating

The carousel steppers both jiggle/vibrate but don't rotate smoothly. Likely caused by a missing connection in the data cables/the data cables being ordered incorrectly.

### Next steps

Check the connections in the base of the robot (L298 connections and pins 8-11). Try just pushing them in, even if they seem okay. The ordering should always be right as the wires are taped. Also check the power wires coming out of the L298. There should be 2 of the same colour in each output.

---

## Arduino restarting

Any strange restarting behaviour on the Arduino is likely due to too much current being drawn through the Arduino's 3.3/5V connections, causing it to restart.

### Next steps

Move potentially high-current connections to a 3.3/5V connection of something else like an L298

---

## LED flashing (500ms)

The LED flashing with a total period of `500ms` indicates that an initial message has not been received from the controller.

### Next steps

* Check that the bluetooth module has connected (fixed lights)
* Check that the bluetooth module is plugged in correctly
* Check for debug messages on the controller
* Run `test/connection` for more info

## LED flashing (200ms)

The LED flashing with a total period of `200ms` indicates that an error has been encountered. It will definitely have been sent as a log message. If no log message has been received, it's likely an issue with the connection.

### Next steps (if no log message)

* Check that the bluetooth module has connected (fixed lights)
* Check that the bluetooth module is plugged in correctly
* Run `test/connection` to debug connection

## LED flashing (1000ms)

The LED flashing with a total period of `1000ms` indicates a buffer overflow (this should've been logged).

### Next steps

* Debug the messages sent to the Arduino, and try to reduce the number of messages sent

### Next steps (if no log message)

* Check that the bluetooth module has connected (fixed lights)
* Check that the bluetooth module is plugged in correctly
* Run `test/connection` to debug connection
