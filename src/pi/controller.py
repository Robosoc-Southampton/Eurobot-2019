
from math import floor, pi

class PathFinder:
	def pathfind(self, from_position, from_facing, to_position, to_facing):
		return [
			("turn", floor((to_facing.to_angle() - from_facing.to_angle()) * 180 / pi)),
			("forward", floor((to_position - from_position).length()))
		]

class Controller:
	def __init__(self, pathfinder):
		self.pathfinder = pathfinder
		self.init()

	def init(self):
		pass

	def run_from(self, state):
		return []
