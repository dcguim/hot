from ant import Ant
from hamilt import obj, inPath
from threading import Lock, Condition
from sys import maxsize
from random import choice 

class Colony():  # required params
    def __init__(self,alg,nants,graph,max_iterations,evap_rate,rand_init,
                 # optional params
                 nelit,alph,beta):
        self.alg = alg
        self.bestobj = maxsize
        self.bestpath = []
        self.size = nants
        self.nelit = nelit
        self.ants = []
        self.rinit = rand_init
        self.maxit = max_iterations
        self.evaprate = evap_rate
        self.alp=alph
        self.bet=beta
        self._g = graph
        self._pheromone = {}
        self.l = Lock()
        self.initPheromoneModel()
        self.init = False
        self.runColony()

    def initPheromoneModel(self):
        for e in self._g.edges:
            self._pheromone[e] = 0.5
        print('pherormone model')
        print(self._pheromone)
        
    def pickRandomNode(self):
        return choice(list(self._g.nodes))
    
    def getPhero(self,i,j):
        if (i,j) in self._pheromone.keys():
            return self._pheromone[(i,j)]
        elif (j,i) in self._pheromone.keys():
            return self._pheromone[(j,i)]
        else:
            print("Unexisting ({0},{1}) pheromone/edge".format(
                str(i),str(j)))
            return 0
        
    def setPhero(self,i,j,v):
        if (i,j) in self._pheromone.keys():
            self._pheromone[(i,j)] = v
        elif (j,i) in self._pheromone.keys():
            self._pheromone[(j,i)] = v
        else:
            print("Unexisting ({0},{1}) pheromone/edge".format(
                str(i),str(j)))
            self._pheromone[(i,j)] = 0

    def univisited(self, init):
        return [n for n in self._g.nodes if n != init]
        
    def getWeight(self,i,j):        
        if  (i,j) in self._g.edges:
            return self._g[i][j]['weight']
        elif (j,i) in self._g.edges:
            return self._g[j][i]['weight']
        else:
            raise ValueError("Unexisting ({0},{1}) weight/edge".format(
                str(i),str(j)))
        
    def pheroEvaporate(self):
        for e in self._g.edges:
            prev_phero = self.getPhero(e[0],e[1])
            next_phero = (1 - self.evaprate)*prev_phero
            self.setPhero(e[0],e[1],next_phero)
        
    def runColony(self):
        print("run colony ...")
        base = 0
        for i in range(self.maxit):
            self.runIt(i)
        print('best path found so far: {0}; objfun: {1}'.format(
            self.bestpath,self.bestobj))

    def runIt(self, i):
        base = self.size*i
        for j in range(self.size):
            # if thread base+j is alive wait for it to terminate,
            # before spawning it again
            print("starting thread {0}".format(j))
            self.ants.append(Ant(base+j+1,self.rinit,self))
            self.ants[base+j].start()
        self.init = True
        for j in range(self.size):
            if self.ants[base+j].is_alive():
                print("is {0} alive?".format(base+j))
                self.ants[base+j].join()


    def neighborhood(self,node,path):
        return [n for n in self._g[node].keys() if n not in path]

    def pheroUpdate(self, tid, path):
        "update the pheromone from thread tid path"
        print("update the pheromone from thread {0}".format(tid))
        self.l.acquire()
        print("thread "+str(tid)+" acquired lock for path:")
        print(path)
        self.pheroEvaporate()
        o = obj(self._g,path)
        if o < self.bestobj:
            self.bestobj = o
            self.bestpath = path
        for i in range(len(path)-1):
            prev_phero = self.getPhero(path[i],path[i+1])
            if self.alg == 'elitist':
                if inPath(path[i],path[i+1],self.bestpath):
                    dElit = 1/self.bestobj
                    self.setPhero(path[i],path[i+1], prev_phero + 1/o + dElit)
                else:
                    self.setPhero(path[i],path[i+1], prev_phero + 1/o)
            else:
                self.setPhero(path[i],path[i+1], prev_phero + 1/o)
        print(self._pheromone)
        self.l.release()
        print("thread "+str(tid)+" released lock" )
