
import lib.comms
import lib.controller
import lib.messages
import lib.state
from lib.position import RobotPosition
import sys
import time

# PRIMARY_ADDRESS = "20:17:03:08:58:54"
# PRIMARY_CONFIG = "src/pi/msgs/config2.txt"
PRIMARY_ADDRESS = "20:17:03:08:60:45"
PRIMARY_CONFIG = "src/pi/msgs/config.txt"
# SECONDARY_ADDRESS = "20:17:03:08:58:54"
# SECONDARY_CONFIG = "src/pi/msgs/config2.txt"

primary_connection = lib.comms.BluetoothConnection(PRIMARY_ADDRESS)
# secondary_connection = lib.comms.BluetoothConnection(SECONDARY_ADDRESS)

if not primary_connection:
	print("Failed to connect to primary")
	sys.exit()

def logger(msg):
	print("[" + str(time.clock()) + "] Log message received (primary): " + msg.strip())

primary_connection.on_log(logger)
# secondary_connection.on_log(logger)
primary_connection.connect()
# secondary_connection.connect()

time.sleep(1)

primary_connection.send_batched(lib.messages.parse_message_file(PRIMARY_CONFIG))
# secondary_connection.send_batched(lib.messages.parse_message_file(SECONDARY_CONFIG))

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

side = getSide()
print(side)

def prepareConfigure():
	primary_configured = [False]
	secondary_configured = [False]

	def on_configure(robot, opcode, data):
		if opcode == "status" and data == 1:
			if robot == "primary":
				primary_configured[0] = True
			elif robot == "secondary":
				secondary_configured[0] = True 

	primary_connection.on_message(lambda opcode, data: on_configure("primary", opcode, data))
	# secondary_connection.on_message(lambda opcode, data: on_configure("secondary") if opcode == 'status' and data == 1 else None)

	while not primary_configured[0]: # or not secondary_configured[0]
		pass

	print("configured")

def configurePrimary():
	messages = []

	# left only for now
	messages.append(('do', 1100))
	messages.append(('forward', 330))
	messages.append(('turn', -90))
	messages.append(('do', 2))
	messages.append(('forward', 205))
	messages.append(('do', 10))
	messages.append(('do', 101))
	messages.append(('do', 102))
	messages.append(('do', 0))
	messages.append(('forward', -245))
	messages.append(('turn', 90))
	messages.append(('forward', -640))
	messages.append(('turn', -90))
	
	messages.append(('echo', 1))
	messages.append(('do', 1000))

	primary_connection.send_batched(messages)

def configureSecondary():
	pass

configurePrimary()
configureSecondary()
prepareConfigure()

primary_connection.on_log(lambda msg: print("[" + str(time.clock()) + "] Log message received: " + msg.strip()))
primary_connection.on_message(lambda opcode, data: print("[" + str(time.clock()) + "] Message received (%s %s)" % (opcode, data)))

# secondary_connection.on_log(lambda msg: print("[" + str(time.clock()) + "] Log message received: " + msg.strip()))
# secondary_connection.on_message(lambda opcode, data: print("[" + str(time.clock()) + "] Message received (%s %s)" % (opcode, data)))

if side == "left":
	primary_connection.send_batched(lib.messages.parse_message_file("src/pi/msgs/primary-left.txt"))
else:
	# TODO: remove
	primary_connection.send_batched(lib.messages.parse_message_file("src/pi/msgs/primary-right.txt"))

while True:
	pass
