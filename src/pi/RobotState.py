
from RobotPosition import RobotPosition

class RobotState:
	def __init__(self, position, facing):
		self.position = position
		self.facing = facing.normalise()
		self.forward_distance = 0

	def forward(self, distance):
		self.forward_distance = distance

	def success(self):
		self.position = self.position.add_scaled(self.facing, self.forward_distance)

	def collision(self, distance):
		self.position = self.position.add_scaled(self.facing, distance)
