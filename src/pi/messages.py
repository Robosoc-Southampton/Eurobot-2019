
import struct

opcodes = {
	"forward": 'F',
	"turn": 'T',
	"do": 'D',
	"align": 'A',
	"request": 'R',
	"message": 'M',
	"collision": 'c',
	"status": 's',
	"return": 'r',
	"log": 'l',
	# "": '',
}

opcodes_inverse = {}

for k, v in opcodes.items():
	opcodes_inverse[ord(v)] = k

def encode_message(opcode, data):
	return struct.pack(">ch", opcodes[opcode][0].encode("ASCII"), data)

def decode_message(data):
	return (opcodes_inverse[data[0]], struct.unpack("<h", data[1:3])[0])

def parse_message(message):
	split = message.split(" ")
	opcode = split[0]
	data = int(split[1])
	return (opcode, data)

def parse_messages(messages):
	messages = []

	for line in str.splitlines():
		if line != "":
			messages.append(parse_message(line))

	return messages

def parse_message_file(file):
	h = open(file, "r")
	content = h.read()
	h.close()
	return parse_messages(content)
