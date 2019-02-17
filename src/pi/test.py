
import comms
import messages
import time

print("connecting bluetooth")
bdaddr = "20:17:03:08:60:45" # comms.find_bt_addr()
print("addr " + bdaddr)
conn = comms.connect(bdaddr)
print("connected")

conn.on_log(lambda msg: print("Log message received: " + msg.strip()))
conn.on_message(lambda opcode, data: print("Message received (%s %s)" % (opcode, data)))

while True:
	inp = input("enter message: ")
	if inp == "": continue
	parsed = messages.parse_message(inp)
	conn.send(messages.encode_message(parsed[0], parsed[1]))
	time.sleep(0.1)
