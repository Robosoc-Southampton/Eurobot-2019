
from lib.position import RobotPosition
from math import pi

class RobotState:
	def __init__(self, position, facing):
		self.position = position
		self.facing = facing.normalise()
		self.forward_distance = 0

	def forward(self, distance):
		self.forward_distance = distance

	def turn(self, angle):
		self.facing = RobotPosition.from_angle(self.facing.to_angle() + angle * pi / 180.0)

	def success(self):
		self.position = self.position.add_scaled(self.facing, self.forward_distance)

	def collision(self, distance):
		self.position = self.position.add_scaled(self.facing, distance)

	def clone(self):
		return RobotState(self.position, self.facing)
