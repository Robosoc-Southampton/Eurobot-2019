
from math import floor, pi

class PathFinder:
	def pathfind(self, from_position, from_facing, to_position):
		return [
			("turn", floor((to_position.sub(from_position).to_angle() - from_facing.to_angle()) * 180 / pi)),
			("forward", floor((to_position.sub(from_position)).length()))
		]

class Controller:
	def __init__(self, pathfinder):
		self.pathfinder = pathfinder
		self.init()

	def init(self):
		pass

	def start(self):
		pass

	def next(self):
		pass

	def complete(self):
		pass

	def begin(self, state):
		self.state = state.clone()
		self.messages = []

	def finish(self):
		return self.messages

	def append(self, messages):
		for message in messages:
			if message[0] == "forward":
				self.state.forward(message[1])
				self.state.success()
			elif message[0] == "turn":
				self.state.turn(message[1])
				while message[1] >  180: message = ("turn", message[1] - 360)
				while message[1] < -180: message = ("turn", message[1] + 360)

			self.messages.append(message)

	def goto(self, position):
		self.append(self.pathfinder.pathfind(self.state.position, self.state.facing, position))

	def face(self, facing):
		self.append([("turn", floor((facing.to_angle() - self.state.facing.to_angle()) * 180 / pi))])

	def run(self):
		if self.start:
			self.start()

		if self.next:
			while self.next():
				pass

		if self.complete:
			self.complete()

		return self.messages
