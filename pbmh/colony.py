from ant import Ant

class Colony():
    def __init__(self,nants,graph,init_node):
        self.size = nants
        self.ants = []
        self.init = init_node
        self.__g = graph
        self.__pheromone = {}

        self.initPheromoneModel()
        self.initColony()

    def initPheromoneModel(self):
        for e in self.__g.edges:
            self.__pheromone[e] = 0.5            
        print(self.__pheromone)

    def getPhero(self,i,j):
        return self.__pheromone[(i,j)]
    
    def getWeight(self,i,j):
        return self.__g[i][j]['weight']
    
    def initColony(self):
        print("init colony ...")
        for i in range(self.size):
            unvisited = [n for n in self.__g.nodes if n != self.init]
            self.ants.append(Ant(i,0,unvisited,self))
            self.ants[i].start()
            print("is alive?")
            print(self.ants[i].is_alive())

    def neighborhood(self,node):
        return [n for n in self.__g[node].keys()]

