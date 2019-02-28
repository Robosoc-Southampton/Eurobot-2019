
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

## LED flashing (+-500ms)

The LED flashing with a total period of `1` second indicates that an initial message has not been received from the controller.

### Next steps

* Check that the bluetooth module has connected (fixed lights)
* Check that the bluetooth module is plugged in correctly
* Check for debug messages on the controller
* Run `test/connection` for more info

## LED flashing (+-200ms)

The LED flashing with a total period of `0.4` seconds indicates that an error has been encountered. It will definitely have been sent as a log message. If no log message has been received, it's likely an issue with the connection.

### Next steps (if no log message)

* Check that the bluetooth module has connected (fixed lights)
* Check that the bluetooth module is plugged in correctly
* Run `test/connection` to debug connection
