
# src/pi

This folder contains code that runs on controllers (Raspberry Pis and laptops)

Libraries are located in `./lib`, and programs in `.`.

## console.py

`console.py` handles debug communication with an Arduino over either a Bluetooth or serial connection.
The program lets you enter messages (if `read` mode is enabled), and will print log messages and messages sent from the Arduino back to the console.

The program lets you enter messages in the form `<name> <data>` e.g. `forward 100` or `config-set 30`.

It features auto-completion of commands, and also allows names to be used for config-keys, e.g. `config-key peak-speed` (also with auto-completion). Press tab to get auto-completion options.

### Usage

```
console.py [options] files
```

> Note that you can run this using `bin/pcon` - this uses the `python3` command, not just `python`. This shorthand is currently only implemented for linux/bash.

> You can, in theory, pipe in messages using `cat file | console.py` however the *much* preferred method is using a file parameter. Commands from files are sent linearly in the order they're passed to the program.

### Files

The files given should not contain the `.txt` extension and must reside in `/src/pi/msgs`, (e.g. `$file` -> `/src/pi/msgs/$file.txt`). `/src/pi/msgs/out` is ignored by git, for local testing.

### Options

All options are optional and have a default value shown below.

#### `addr`

```
-addr address='20:17:03:08:60:45'
```

`address` is a MAC address of a Bluetooth device to connect to, or `auto` (for automatic discovery of the Bluetooth device). It defaults to the address of the `orangeRS` Bluetooth module.

* Only applicable if using a Bluetooth connection.

#### `port`

```
-port port='/dev/ttyACM0'
```

The serial port to connect to.

* Only applicable if using a serial connection.

#### `read`

```
-read option='true'
```

Whether to read messages from stdin.

#### `timeout`

```
-timeout t=None
```

A timeout for the program at which point it will close connections and exit. By default there is no timeout.

#### `serial`

```
-serial use='false'
```

Whether to use a serial connection. If `true`, Bluetooth will not be used to connect. Used in conjunction with `-port`.

## controller.py

