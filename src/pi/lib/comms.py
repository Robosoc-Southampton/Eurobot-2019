#!/usr/bin/env python3

import bluetooth
import threading
import serial
import lib.messages

class Connection:
	def __init__(self):
		self.receive_log_buffer = b""
		self.receive_message_buffer = b""
		self.receive_log_handler = None
		self.receive_message_handler = None
		self.receive_log_length = 0

	# sets the message handler callback
	def on_message(self, handler):
		self.receive_message_handler = handler

	# sets the log handler callback
	def on_log(self, handler):
		self.receive_log_handler = handler

	# establishes the connection
	def connect(self):
		pass

	# sends a message to the arduino
	def send(self, data):
		pass

	# closes the connection
	def close(self):
		pass

	# internal use only
	def check_message_buffer(self):
		while len(self.receive_message_buffer) >= 1 and self.receive_log_length > 0 or len(self.receive_message_buffer) >= 3:
			if self.receive_log_length > 0:
				self.receive_log_buffer = self.receive_log_buffer + self.receive_message_buffer[0:self.receive_log_length]

				if len(self.receive_message_buffer) > self.receive_log_length:
					self.receive_message_buffer = self.receive_message_buffer[self.receive_log_length:]
					self.receive_log_length = 0
				else:
					self.receive_log_length -= len(self.receive_message_buffer)
					self.receive_message_buffer = b""

				if self.receive_log_length == 0:
					if self.receive_log_handler != None:
						self.receive_log_handler(self.receive_log_buffer.decode("ASCII"))

					self.receive_log_buffer = b""

			if len(self.receive_message_buffer) >= 3:
				decoded = lib.messages.decode_message(self.receive_message_buffer)
				if decoded[0] == "log":
					self.receive_log_length = decoded[1]
				elif self.receive_message_handler != None:
					self.receive_message_handler(decoded[0], decoded[1])

				self.receive_message_buffer = self.receive_message_buffer[3:]

class BluetoothConnection(Connection):
	def __init__(self, bdaddr, port=1):
		Connection.__init__(self)
		self.bdaddr = bdaddr
		self.port = port
		
	def connect(self):
		self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
		self.sock.connect((self.bdaddr, self.port))

		listener = BTConnectionListener(self)
		listener.setDaemon(True)
		listener.start()

	def send(self, data):
		self.sock.send(data)

	def close(self):
		self.sock.close()

class BTConnectionListener(threading.Thread):
	def __init__ (self, conn):
		threading.Thread.__init__(self)
		self.conn = conn

	def run(self):
		try:
			while True:
				data = self.conn.sock.recv(1024)
				if len(data) == 0: break
				self.conn.receive_message_buffer += data
				self.conn.check_message_buffer()
		except IOError:
			pass

		self.conn.close()

class SerialConnection(Connection):
	def __init__(self, port):
		Connection.__init__(self)

		self.port = port

	def connect(self):
		self.ser = serial.Serial(self.port,
			baudrate=9600,
			bytesize=serial.EIGHTBITS,
			parity=serial.PARITY_NONE,
			stopbits=serial.STOPBITS_ONE,
			xonxoff=0,
			rtscts=0
			)
		self.ser.setDTR(False)

		# wait for initial message from wait_for_connection()
		while self.ser.in_waiting < 3:
			pass

		self.ser.read(3)
		
		listener = SerialConnectionListener(self)
		listener.setDaemon(True)
		listener.start()

	def send(self, data):
		self.ser.write(data)

	def close(self):
		self.ser.close()

class SerialConnectionListener(threading.Thread):
	def __init__ (self, conn):
		threading.Thread.__init__(self)
		self.conn = conn

	def run(self):
		try:
			while True:
				data = self.conn.ser.read()
				self.conn.receive_message_buffer += data
				self.conn.check_message_buffer()
		except IOError:
			pass

		self.conn.close()

def find_bt_addr():
	nearby_devices = bluetooth.discover_devices()

	for bdaddr in nearby_devices:
		if bluetooth.lookup_name(bdaddr) != None and bluetooth.lookup_name(bdaddr).endswith("RS"):
			return bdaddr

	return None
