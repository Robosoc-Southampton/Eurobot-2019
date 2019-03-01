
from math import sin, cos, sqrt, atan2, pi

class RobotPosition:
	def from_angle(angle):
		return RobotPosition(sin(angle), cos(angle))

	def __init__(self, x, y):
		self.x = x
		self.y = y

	def __str__(self):
		return "(" + str(self.x) + ", " + str(self.y) + ")"

	def __len__(self):
		return sqrt(self.x ^ 2 + self.y ^ 2)

	def __repr__(self):
		return str(self)

	def add(self, other):
		return RobotPosition(self.x + other.x, self.y + other.y)

	def sub(self, other):
		return RobotPosition(self.x - other.x, self.y - other.y)

	def add_scaled(self, other, s):
		return RobotPosition(self.x + other.x * s, self.y + other.y * s)

	def length(self):
		return len(self)

	def normalise(self):
		l = 1/self.length()
		return RobotPosition(self.x * l, self.y * l)

	def to_angle(self):
		angle = atan2(self.x, self.y)
		while angle < 0: angle += 2 * pi
		return angle

RobotPosition.zero = RobotPosition(0, 0)
RobotPosition.one = RobotPosition(1, 1)
