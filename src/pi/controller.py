import lib.comms
import lib.controller
import lib.messages
import lib.state
from lib.position import RobotPosition
import sys
import time
import socket
import math

PRIMARY_ADDRESS = "20:17:03:08:60:45"
PRIMARY_CONFIG = "src/pi/msgs/config.txt"
# SECONDARY_ADDRESS = "20:17:03:08:58:54"
SECONDARY_CONFIG = "src/pi/msgs/config2.txt"

VISION_PORT = 12345
VISION_TCP_BUFFER_SIZE = 1024

if "-dp" in sys.argv:
	primary_connection = lib.comms.DummyConnection()
else:
	primary_connection = lib.comms.BluetoothConnection(PRIMARY_ADDRESS)

if "-ds" in sys.argv:
	secondary_connection = lib.comms.DummyConnection()
else:
	# secondary_connection = lib.comms.BluetoothConnection(SECONDARY_ADDRESS)
	secondary_connection = lib.comms.SerialConnection("/dev/ttyACM0")

if not primary_connection:
	print("Failed to connect to primary")
	sys.exit()

if not secondary_connection:
	print("Failed to connect to secondary")
	sys.exit()

primary_connection.on_log(lambda msg: print("[" + str(time.clock()) + "] Log message received (primary): " + msg.strip()))
secondary_connection.on_log(lambda msg: print("[" + str(time.clock()) + "] Log message received (secondary): " + msg.strip()))
primary_connection.connect()
secondary_connection.connect()

time.sleep(1)

primary_connection.send_batched(lib.messages.parse_message_file(PRIMARY_CONFIG))
secondary_connection.send_batched(lib.messages.parse_message_file(SECONDARY_CONFIG))

time.sleep(1)

def getSide():
	readings = []
	turns = 0

	primary_connection.on_message(lambda opcode, data: readings.append(data) if opcode == 'return' else None)
	primary_connection.send_batched([
		# turn 90 and request reading
		('turn', 90),
		('do', 0),
		('request', 1001),
		# turn other 90 and request reading
		('turn', -180),
		('do', 0),
		('request', 1001),
		# face forward and request reading (just for ease of tracking where it is in this code)
		('turn', 90),
		('do', 0),
		('request', 1001)
	])

	# wait for all readings to come through
	while len(readings) < 3:
		pass

	# log side readings
	print("to left: ", readings[0])
	print("to right: ", readings[1])

	# reset the message buffer
	primary_connection.send(('message', 7))

	# return left if it was closer
	return 'left' if readings[0] < readings[1] else 'right'

# side = getSide()
side = "left" if "left" in sys.argv else "right"
print("on side: ", side)

def configurePrimary():
	messages = []

	messages.append(('do', 1100))

	if side == "left":
		messages.append(('turn', -90))
		messages.append(('do', 252))
		messages.append(('do', 0))
		messages.append(('forward', 50))
		messages.append(('turn', 91))
		messages.append(('forward', 1080))
		messages.append(('do', 251))
		messages.append(('do', 0))
		messages.append(('forward', -1200))
		messages.append(('turn', -90))
		messages.append(('do', 2))
		messages.append(('do', 252))
		messages.append(('do', 0))
		messages.append(('forward', 100))
	else:
		messages.append(('turn', 90))
		messages.append(('do', 252))
		messages.append(('do', 0))
		messages.append(('forward', 50))
		messages.append(('turn', -91))
		messages.append(('forward', 1080))
		messages.append(('do', 251))
		messages.append(('do', 0))
		messages.append(('forward', -1200))
		messages.append(('turn', 90))
		messages.append(('do', 2))
		messages.append(('do', 252))
		messages.append(('do', 0))
		messages.append(('forward', 100))

	messages.append(('echo', 1))
	messages.append(('do', 1000))

	primary_connection.send_batched(messages)

def configureSecondary():
	messages = []

	messages.append(('do', 1100))

	if side == "left":
		messages.append(('turn', -90))
		messages.append(('forward', 330))
		messages.append(('forward', -320))
		messages.append(('turn', 90))
		messages.append(('forward', -10))
		messages.append(('forward', 10))
	else:
		messages.append(('turn', 90))
		messages.append(('forward', 330))
		messages.append(('forward', -320))
		messages.append(('turn', -90))
		messages.append(('forward', -10))
		messages.append(('forward', 10))

	messages.append(('echo', 1))
	messages.append(('do', 1000))

	secondary_connection.send_batched(messages)

def waitForConfigure():
	primary_configured = ["-dp" in sys.argv]
	secondary_configured = ["-ds" in sys.argv]

	def on_configure(robot, opcode, data):
		if opcode == "status" and data == 1:
			if robot == "primary":
				primary_configured[0] = True
			elif robot == "secondary":
				secondary_configured[0] = True

	primary_connection.on_message(lambda opcode, data: on_configure("primary", opcode, data))
	secondary_connection.on_message(lambda opcode, data: on_configure("secondary", opcode, data))

	while not primary_configured[0] or not secondary_configured[0]:
		pass

	print("configured")

def seeAtoms(s, vision_message):
	print("Here1")
	s.send(bytes(vision_message + "\n", 'utf-8'))
	data = b""
	while(data == "" or data[-2:] != "\n\n"):
		data += s.recv(VISION_TCP_BUFFER_SIZE)
	data = data[:-2]
	point_strings = data.split(b'\n')

	print("Here2")

	points = []

	for point_string in point_strings:
		coords = point_string.split(b",")
		points.append((int(float(coords[0])), int(float(coords[1]))))

	print("Here3")

	return points

def collectAtom(s, vision_message):
	print("Looking for atom")
	atoms = seeAtoms(s, vision_message)
	atoms = offsetAtoms(atoms)
	atom = nearestAtom(atoms)

	(x,y) = atom
	angle = math.atan2(x, y)
	angle = math.degrees(angle)
	angle = int(angle)
	angle *= 1
	dist = math.hypot(x, y)
	dist = int(dist)
	dist -= 100

	#turn angle
	#go distance (- something)
	messages = []
	print("Found atom")
	messages.append(("turn", angle))
	messages.append(("forward", dist))
	secondary_connection.send_batched(messages)


def nearestAtom(atoms):
	nearest = None
	dist = 1000000

	for atom in atoms:
		thisDist = math.hypot(atom[0], atom[1])
		if thisDist < dist:
			nearest = atom
			dist = thisDist
	return nearest


def offsetAtoms(atoms):
	transformed = list()
	for (x,y) in atoms:
		transformed.append((100-x, 200+y))
	return transformed

def collectRedium(s):
	collectAtom(s, "findRedium")

def collectGreenium(s):
	collectAtom(s, "findGreenium")

def collectBlueium(s):
	collectAtom(s, "findBlueium")

def doVision(opcode, data):
	if opcode == "status" and data == 1234:
		secondary_connection.send(('message', 7))
		secondary_connection.send(('do', 1))
		secondary_connection.send(('do', 1))
		collectGreenium(s)

s = None

if not "-ds" in sys.argv:
	ip = 'localhost'
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((ip, VISION_PORT))

configurePrimary()
configureSecondary()
waitForConfigure()

primary_connection.send(("message", 7))
secondary_connection.send(("message", 7))

primary_connection.on_message(lambda opcode, data: print("[" + str(time.clock()) + "] (primary) Message received (%s %s)" % (opcode, data)))
#secondary_connection.on_message(lambda opcode, data: print("[" + str(time.clock()) + "] (secondary) Message received (%s %s)" % (opcode, data)))
secondary_connection.on_message(doVision)

if side == "left":
	primary_connection.send_batched(lib.messages.parse_message_file("src/pi/msgs/primary-left.txt"))
else:
	primary_connection.send_batched(lib.messages.parse_message_file("src/pi/msgs/primary-right.txt"))

if side == "left":
	secondary_connection.send_batched(lib.messages.parse_message_file("src/pi/msgs/secondary-left.txt"))
else:
	secondary_connection.send_batched(lib.messages.parse_message_file("src/pi/msgs/secondary-right.txt"))

while True:
	pass
