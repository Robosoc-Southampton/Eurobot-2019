
import lib.comms
import lib.messages
import bluetooth
import sys
import time
import readline
import traceback

options = {
	"addr": "20:17:03:08:60:45",
	"port": "/dev/ttyACM0",
	"files": [],
	"read": "true",
	"timeout": None,
	"serial": "false"
}
next_opt = None

for opt in sys.argv[1:]:
	if next_opt:
		options[next_opt] = opt
		next_opt = None
	elif opt.startswith("-"):
		if opt[1:] in options:
			next_opt = opt[1:]
		else:
			raise "nope"
	else:
		options["files"].append(opt)

bdaddr = options["addr"]

if bdaddr == "auto":
	bdaddr = comms.find_bt_addr()

	if bdaddr == None:
		print("Couldn't find bluetooth device")
		sys.exit()


if options["serial"] == "true":
	print("Connecting serial")

	conn = lib.comms.SerialConnection(options["port"])
else:
	print("Connecting bluetooth")
	print("Addr: " + bdaddr)

	try:
		conn = lib.comms.BluetoothConnection(bdaddr)
	except bluetooth.btcommon.BluetoothError as e:
		print("Failed to connect bluetooth: " + str(e))
		sys.exit()

if not conn:
	print("Failed to connect!")
	sys.exit()

conn.on_log(lambda msg: print("Log message received: " + msg.strip()))
conn.on_message(lambda opcode, data: print("Message received (%s %s)" % (opcode, data)))
conn.connect()
conn.send(("message", 1))

print("Connected")

start_time = time.clock()

file_messages = []

for file in options["files"]:
	print("Loading file %s" % file)
	for msg in lib.messages.parse_message_file(file):
		file_messages.append(msg)

if len(file_messages) > 0:
	conn.send_batched(file_messages)

if options["read"] == "true":
	print("Enter 'q' to quit")

	readline.set_auto_history(True)

	def complete(options, begin, text, state):
		values = []

		for s in options:
			if (s.startswith(text)):
				values.append(s)

		if state < len(values):
			return begin + values[state]

	def completer(text, state):
		if text.find(" ") == -1:
			return complete(lib.messages.opcodes.keys(), "", text, state)

		if text.startswith("config-key "):
			return complete(lib.messages.config_keys.keys(), "config-key ", text[11:], state)

	readline.set_completer_delims("")
	readline.parse_and_bind("tab: complete")
	readline.set_completer(completer)

while True:
	try:
		if options["timeout"] != None and time.clock() - start_time >= options["timeout"]:
			break

		if options["read"] == "true":
			inp = input("enter message: ")

			if inp == "q":
				break

			if inp.find(" ") == -1:
				print("Invalid command")
				continue

			parsed = lib.messages.parse_message(inp)
			print("Sending (" + lib.messages.opcodes[parsed[0]] + " " + str(parsed[1]) + ")")
			conn.send(parsed)
			time.sleep(1)
	except KeyboardInterrupt:
		print("")
		break
	except ValueError:
		print("Enter a number parameter")
	except:
		print("An error occurred!")
		print(repr(sys.exc_info()[1]))

conn.close()
