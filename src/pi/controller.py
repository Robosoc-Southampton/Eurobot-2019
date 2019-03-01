
import lib.controller
import lib.messages
import lib.state
from lib.position import RobotPosition
import sys

files = sys.argv[1:]

position = RobotPosition(0, 0)
facing = RobotPosition(0, 1)
state = lib.state.RobotState(position, facing)
pathfinder = lib.controller.PathFinder()            # TODO: add other pathfinders
controller = lib.controller.Controller(pathfinder)  # TODO: add other controllers

controller.begin(state)

for file in files:
	h = open(file, "r")
	content = h.read()
	h.close()

	for line in content.splitlines():
		if line.startswith("goto "):
			parts = line[5:].split(", ")
			controller.goto(RobotPosition(int(parts[0].strip()), int(parts[1].strip())))
		elif line.startswith("face"):
			parts = line[5:].split(", ")
			controller.face(RobotPosition(int(parts[0].strip()), int(parts[1].strip())))
		else:
			controller.append(lib.messages.parse_messages(line))

controller.run()

print(lib.messages.serialise_messages(controller.finish()))
