
# RoboSoc - Eurobot-2019

This repo will be used to track issues, host code for the robots, and host any shared design documents that aren't on other services.

### Useful links

- [Slack](https://robosoc-eurobot.slack.com)
  - Where we discuss stuff (and memes).
- [Eurobot webpage](http://www.eurobot.org/eurobot/eurobot-2019)
  - The official page for this year's competition.
- [Eurobot spec](http://www.eurobot.org/images/2019/Eurobot2019_Rules_Cup_OFFICIAL_EN.pdf)
  - The official, full specification for this year's competition.
- [Summarised spec](https://hackmd.io/2y2AnXZyQdaL18kg8T7bjg)
  - Our own summary of the specification.

### Compilation/uploading

> You'll need the Arduino IDE installed already. You can get this [here](https://www.arduino.cc/en/Main/Software).

Because we're using two robots, this project doesn't use the typical process of uploading sketches for Arduino.

In [bin/](bin), there are utility files which wrap around the Arduino-IDE's command line interface and allow the code for both robots to be compiled/uploaded.

> Note: `bin/*` are set-up to build for Arduino UNO.

* Use `verify` for validating the code (seeing if it compiles)
* Use `upload` to upload the code to the board
* Use `ports` to list ports
  * Note that on Windows this outputs more information than is needed. Look for `COMxx`, that's the port you should use.

`verify` and `upload` both take a `MODE` argument. This should be either `primary` or `secondary` and will create a `src/arduino/robot.ino` file prior to compilation[/upload].

#### For Windows

* `bin/verify (primary|secondary)`
* `bin/upload PORT (primary|secondary)`
* `bin/ports`

#### For Linux

Not yet available. See [bin/](bin).
