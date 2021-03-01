from math import sqrt
import collections

#The node class, used for storing points
class Node:
    def __init__(self, Name, Position):
        self.Name = Name
        self.Position = Position
        self.Wall = False

#The graph class which defines where nodes are, who they are connected to and how far away they are from another node
class Graph:
    def __init__(self):
        self.edges = {}
        self.weights = {}

    def Neighbors(self, id):
        return self.edges[id]

    def Weight(self, Node1, Node2):
        Cost = sqrt(((Node1.Position[0] - Node2.Position[0])**2) + ((Node1.Position[1] - Node2.Position[0])**2))
        return self.weights.get(Node2, Cost)

#An example class to allow for testing of the pathfinding
class ExampleGraph:
    def __init__(self):
        self.A = Node("Node A", (1, 1))
        self.B = Node("Node B", (3, 3))
        self.C = Node("Node C", (4, 4))
        self.D = Node("Node D", (3, 5))
        self.E = Node("Node E", (5, 5))
        self.F = Node("Node F", (1, 2))
        self.G = Node("Node G", (4, 5))

        self.ExampleGraphBetterName = Graph()
        self.ExampleGraphBetterName.edges = {
            self.A: [self.B],
            self.B: [self.A, self.C, self.D],
            self.C: [self.A],
            self.D: [self.E, self.A],
            self.E: [self.B],
            self.F: [self.A, self.E],
            self.G: [self.A, self.B, self.C, self.D, self.E, self.F]
        }

class Grid:
    def __init__(self):
        self.x = 1
        self.y = 1
        self.NodeList = {}
        self.GraphGrid = Graph()

    def CreateNodeGrid(self):
        while(self.y < 101):
            if self.x < 101:
                self.NodeList["Node x = " + str(self.x) + " y = " + str(self.y)] = Node(str("Node x = " + str(self.x) + " y = " + str(self.y)), (self.x, self.y))
                self.x += 1
            else:
                self.x = 1
                self.y += 1

class GeneralGrid:
    def __init__(self):
        self.x = 1
        self.y = 1
        self.NodeList = {}
        self.weights = {}
        self.GraphGrid = Graph()

    def CreateNodeGrid(self, SizeX, SizeY):
        while(self.y < SizeY):
            if self.x < SizeX:
                self.NodeList["Node x = " + str(self.x) + " y = " + str(self.y)] = Node(str("Node x = " + str(self.x) + " y = " + str(self.y)), (self.x, self.y))
                self.x += 1
            else:
                self.x = 1
                self.y += 1

    def Neighbors(self, id):
        Results = []
        Pos = id.Position
        NeighborLeft = self.NodeList.get("Node x = " + str((Pos[0] - 1)) + " y = " + str(Pos[1]))
        NeighborRight = self.NodeList.get("Node x = " + str((Pos[0] + 1)) + " y = " + str(Pos[1]))
        NeighborBelow = self.NodeList.get("Node x = " + str(Pos[0]) + " y = " + str(Pos[1] - 1))
        NeighborAbove = self.NodeList.get("Node x = " + str(Pos[0]) + " y = " + str(Pos[1] + 1))
        if isinstance(NeighborLeft, Node):
            Results.append(NeighborLeft)
        if isinstance(NeighborRight, Node):
            Results.append(NeighborRight)
        if isinstance(NeighborBelow, Node):
            Results.append(NeighborBelow)
        if isinstance(NeighborAbove, Node):
            Results.append(NeighborAbove)
        return Results

    def Weight(self, Node1, Node2):
        Cost = sqrt(((Node1.Position[0] - Node2.Position[0])**2) + ((Node1.Position[1] - Node2.Position[0])**2))
        return self.weights.get(Node2, Cost)

    def Wall(self, Node):
        Node.wall = True



#A queue class which allows us to store where we have been
class Queue:
        def __init__(self):
            self.elements = collections.deque()

        def empty(self):
            return len(self.elements) == 0

        def put(self, x):
            self.elements.append(x)

        def get(self):
            return self.elements.popleft()

#The a star pathfinding algorithem
def AStartSearch(Graph , Start, Goal):
    frontier = Queue()
    frontier.put(Start)
    CameFrom = {}
    CameFrom[Start] = None
    CostSoFar = 0
    Visited = {}
    Visited[Start] = True

    while not frontier.empty():
        Current = frontier.get()

        if Current == Goal:
            break

        for Next in Graph.Neighbors(Current):
            if Next.Wall == False:
                NewCost = CostSoFar + Graph.Weight(Current, Next)
                if Next not in CameFrom or NewCost < CostSoFar:
                    CostSoFar += NewCost
                    frontier.put(Next)
                    CameFrom[Next] = Current
                    print("Visiting %r" % Current.Name)

    return  CameFrom

TestGraph =  ExampleGraph()
Testing = AStartSearch(TestGraph.ExampleGraphBetterName, TestGraph.A, TestGraph.E)

#A function that takes in the path from the pathfinding and reconstructs it
def ReconstructPath(CameFrom, Start, Goal):
    current = Goal
    path = []
    while current != Start:
        path.append(current)
        current = CameFrom[current]
    path.append(Start)
    path.reverse()
    return path

#A function that takes in the reconstructed path and tells the robot where to move
def TellRobot(Path):
    NodeCounter = 0
    CurrentPos = list(Path[NodeCounter].Position)
    ListOfThingsToDo = []
    while tuple(CurrentPos) != Path[-1].Position:
        NextPos = Path[NodeCounter + 1].Position
        if tuple(CurrentPos) == Path[NodeCounter].Position:
            NodeCounter += 1
        if CurrentPos[0] < NextPos[0]:
            ListOfThingsToDo.append("F")
            CurrentPos[0] += 1
        elif CurrentPos[0] > NextPos[0]:
            ListOfThingsToDo.append("B")
            CurrentPos[0] -= 1
        elif CurrentPos[1] < NextPos[1]:
            ListOfThingsToDo.append("R")
            CurrentPos[1] += 1
        elif CurrentPos[1] > NextPos[1]:
            ListOfThingsToDo.append("L")
            CurrentPos[1] -= 1

    return ListOfThingsToDo



Path = ReconstructPath(Testing, TestGraph.A, TestGraph.E)
print(Path)

ListOfThings = TellRobot(Path)
print(ListOfThings)

TBT = GeneralGrid()
TBT.CreateNodeGrid(10,10)
print(len(list((TBT.NodeList.values()))))
TBTTest = AStartSearch(TBT, list(TBT.NodeList.values())[0], list(TBT.NodeList.values())[70])
PathTBT = ReconstructPath(TBTTest, list(TBT.NodeList.values())[0], list(TBT.NodeList.values())[70])
ListOfThings3 = TellRobot(PathTBT)
print(ListOfThings3)