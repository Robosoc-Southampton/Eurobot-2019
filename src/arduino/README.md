
# src/arduino

This folder contains all the code that runs on the Arduinos.

Headers are located in `./include`, robot and test source files in `./robot` and `./robot/test` respectively, and library/component sources in `.`.

Headers are commented with descriptions of functions, classes, and general code. Source files are largely undocumented.

## Compiling/uploading.

Verifying and uploading code is done using programs in the `bin` folder. Note that, prior to compilation, the entry point (e.g. `primary`, `test/md25`) will be copied into `robot.ino`.

There are two tools, `bin/verify` and `bin/upload`.

### Entry points

An "entry point" is a file you want to upload, e.g. `primary`, or `test/md25`.

> Note, the `.c` extension is not included with the file name.

> On Linux, using `bin/verify` or `bin/upload` will list available entry points

### `bin/verify`

`bin/verify` takes an entry point to compile, and compiles it using Arduino.
This can be used to check code compiles, without running it on a physical Arduino.

### `bin/upload`

`bin/upload` takes a port and an entry point to compile, e.g. `COM3 primary`, or `ACM0 test/md25`, and compiles/uploads it using the Arduino IDE.

To find the port, you can use `bin/ports`. It will list all valid ports.

> On Linux, the port can be omitted, and it will detect an Arduino currently plugged in.
