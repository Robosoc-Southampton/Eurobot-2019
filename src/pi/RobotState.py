
from RobotPosition import RobotPosition

class RobotState:
	def __init__(self, position, facing):
		self.position = position
		self.facing = facing.normalise()
		self.next_position = position
		self.next_facing = facing.normalise()

	def forward(self, distance):
		self.next_position = self.position.add_scaled(self.facing, distance)
		self.next_facing = self.facing

	def turn(self, angle):
		self.next_position = self.position
		self.next_facing = RobotPosition.from_angle(self.facing.to_angle() + angle)

	def success(self):
		self.position = self.next_position
		self.facing = self.next_facing

	def collision(self, distance):
		self.position = self.position.add_scaled(self.facing, distance)
