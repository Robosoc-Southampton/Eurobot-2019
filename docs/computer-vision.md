# Overview

The computer vision system is a Java program that should run alongside
the main Python control program on the Pi. Its job is to find the
locations of different atoms within the view of the Pi's camera.

# How to run

java -jar vision.jar PORT CAMERA -Xmx700m

e.g.

java -jar vision.jar 12345 -1 -Xmx700m

PORT can be any free port.

CAMERA can be either -1 (for pi camera) or the index of the USB webcam
you want to use.

# Interface

Connect to the program using TCP on the specified port. Send it
commands and it will reply with the result. This can be done using the
telnet utility for direct control.

## Commands

### calibrateIntrinsics

This takes 10 exposures of a chessboard pattern to calibrate the
intrinsic parameters of the camera, and stores the calibration stuff
in points.dat. This has already been done for the Pi camera, so you
don't need to do it again. If you do, move the camera around relative
to the chessboard between exposures.

The responses will tell you whether each exposure found a chessboard.

### calibrateExtrinsics

This takes 1 exposure and attempts to find a chessboard in it and
calibrate the extrinsic parameters of the camera (its position and
rotation relative to the plane of the chessboard). This will need to
be redone each time the position of the camera on the robot is
changed.

The response will tell you whether the calibration was a success or a
failure.

### findRedium, findGreenium, findBlueium

Finds atoms of the relevant colour. Returns a list of points, with
each line of the response being a point with the x and y coordinates
separated by a comma. The final line of the response is blank to
signal that it is finished. Example below.

----
findRedium
100,200
200,100
150,150

----

Each point is the position of an atom in mm relative to a corner of
the chessboard.
