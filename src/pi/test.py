
import comms
import messages

print("connecting bluetooth")
bdaddr = comms.find_bt_addr()
print("addr " + bdaddr)
conn = comms.connect(bdaddr)

conn.on_log(lambda msg: print("Log message received: " + msg))

print("connected, sending string")
conn.send(messages.encode_message("forward", 10))

print("waiting for responses")
while True: pass
