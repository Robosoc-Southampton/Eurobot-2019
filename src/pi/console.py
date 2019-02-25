
import comms
import messages
import sys
import time

options = {
	"addr": "20:17:03:08:60:45",
	"file": None,
	"read": "true",
	"timeout": None,
	"serial": "false"
}
next_opt = None

for opt in sys.argv:
	if next_opt:
		options[next_opt] = opt
	elif opt.startswith("-"):
		if opt[1:] in options:
			next_opt = opt[1:]
		else:
			raise "nope"

bdaddr = options["addr"]

if bdaddr == "auto":
	bdaddr = comms.find_bt_addr()

	if bdaddr == None:
		print("couldn't find bluetooth device")
		sys.exit()

print("connecting bluetooth")
print("addr: " + bdaddr)
conn = comms.connect_serial() if options["serial"] == "true" else comms.connect_bluetooth(bdaddr)

if conn:
	print("connected")
else:
	print("failed to connect!")
	sys.exit()

conn.on_log(lambda msg: print("Log message received: " + msg.strip()))
conn.on_message(lambda opcode, data: print("Message received (%s %s)" % (opcode, data)))

start_time = time.clock()

if options["file"] != None:
	print("Loading file %s" % options["file"])
	for msg in messages.parse_message_file(options["file"]):
		conn.send(messages.encode_message(msg[0], msg[1]))

if options["read"] == "true":
	print("Enter 'q' to quit")

while True:
	if options["timeout"] != None and time.clock() - start_time >= options["timeout"]:
		break

	if options["read"] == "true":
		inp = input("enter message: ")
		if inp == "q":
			conn.close()
			break
		if inp.find(" ") == -1:
			continue
		parsed = messages.parse_message(inp)
		conn.send(messages.encode_message(parsed[0], parsed[1]))
		time.sleep(0.5)
