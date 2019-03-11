
import struct

opcodes = {
	"forward": 'F',
	"turn": 'T',
	"do": 'D',
	"request": 'R',
	"message": 'M',
	"config-key": "K",
	"config-set": "S",
	"collision": 'c',
	"status": 's',
	"return": 'r',
	"log": 'l',
	# "": '',
}

config_keys = {
	"robot-radius": 1,
	"wheel-radius": 2,
	"peak-speed": 3,
	"acceleration": 4,
}

opcodes_inverse = {}

for k, v in opcodes.items():
	opcodes_inverse[ord(v)] = k

def encode_message(message):
	if len(message[0]) == 1:
		return struct.pack("<ch", message[0][0].encode("ASCII"), message[1])
	else:
		return struct.pack("<ch", opcodes[message[0]][0].encode("ASCII"), message[1])

def decode_message(data):
	return (opcodes_inverse[data[0]], struct.unpack("<h", data[1:3])[0])

def parse_message(message):
	split = message.split(" ")
	opcode = split[0]

	if opcode == "config-key" and split[1] in config_keys:
		return (opcode, config_keys[split[1]])
	
	data = int(split[1])

	return (opcode, data)

def parse_messages(messages):
	msgs = []

	for line in messages.splitlines():
		if line.find("//") != -1:
			line = line[0:line.index("//")]
		if line != "":
			msgs.append(parse_message(line))

	return msgs

def parse_message_file(file):
	h = open(file, "r")
	content = h.read()
	h.close()
	return parse_messages(content)

def serialise_messages(messages):
	result = []

	for message in messages:
		result.append(message[0] + " " + str(message[1]))

	return '\n'.join(result)
