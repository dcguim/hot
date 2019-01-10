from ant import Ant
from hamilt import obj
from threading import Lock
class Colony():
    def __init__(self,nants,graph,max_iterations,evap_rate,rand_init):
        self.size = nants
        self.ants = []
        self.rinit = rand_init
        self.maxit = max_iterations
        self.evaprate = evap_rate
        self.__g = graph
        self.__pheromone = {}
        self.l = Lock()
        self.initPheromoneModel()
        self.runColony()

    def initPheromoneModel(self):
        for e in self.__g.edges:
            self.__pheromone[e] = 0.5
        print('pherormone model')
        print(self.__pheromone)

    def getPhero(self,i,j):
        if (i,j) in self.__pheromone.keys():
            return self.__pheromone[(i,j)]        
        elif (j,i) in self.__pheromone.keys():
            return self.__pheromone[(j,i)]
        else:
            raise ValueError("Unexisting ({0},{1}) pheromone/edge".format(str(i),str(j)))
        
    def setPhero(self,i,j,v):
        if (i,j) in self.__pheromone.keys():
            self.__pheromone[(i,j)] = v       
        elif (j,i) in self.__pheromone.keys():
            self.__pheromone[(j,i)] = v
        else:
            raise ValueError("Unexisting ({0},{1}) pheromone/edge".format(str(i),str(j)))

    def univisited(self, init):
        return [n for n in self.__g.nodes if n != init]
        
    def getWeight(self,i,j):        
        if  (i,j) in self.__g.edges:
            return self.__g[i][j]['weight']
        elif (j,i) in self.__g.edges:
            return self.__g[j][i]['weight']
        else:
            raise ValueError("Unexisting ({0},{1}) weight/edge".format(str(i),str(j)))
        
    def pheroEvaporate(self):
        for e in self.__g.edges:
            prev_phero = self.getPhero(e[0],e[1])
            next_phero = (1 - self.evaprate)*prev_phero
            self.setPhero(e[0],e[1],)
        
    def runColony(self):
        print("run colony ...")        
        for i in range(self.maxit):
            for j in range(self.size):            
                self.ants.append(Ant(j,self.rinit,self))
                self.ants[j].start()
                print("is alive?")
                print(self.ants[j].is_alive())
            self.pheroEvaporate()

    def neighborhood(self,node,path):
        return [n for n in self.__g[node].keys() if n not in path]

    def pheroUpdate(self, tid, path):
        "update the pheromone from thread`s path"
        self.l.acquire()
        print("thread "+str(tid)+" acquire lock for path:")
        print(path)
        o = obj(self.__g,path)
        for i in range(len(path)-1):
            prev_phero = self.getPhero(path[i],path[i+1])
            self.setPhero(path[i],path[i+1], prev_phero + 1/o)
        print(self.__pheromone)        
        self.l.release()
        print("thread "+str(tid)+" release" )
