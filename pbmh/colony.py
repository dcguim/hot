from ant import Ant
from hamilt import obj, inPath
from threading import Lock, Condition
from sys import maxsize
from random import choice
from math import log, ceil

class Colony():  # required params
    def __init__(self,alg,nants,graph,max_iterations,evap_rate,rand_init,
                 # optional params
                 nelit,alph,beta,rank):
        self.alg = alg
        self.bestobj = maxsize
        self.bestpath = []
        self.size = nants
        self.nelit = nelit
        self.rank = rank
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
        self.rankedants = []
        if self.alg == 'rank':
            for r in range(self.rank):
                self.rankedants.append([[],maxsize])
        self.init = False
        self.runColony()

    def initPheromoneModel(self):
        for e in self._g.edges:
            self._pheromone[e] = 0.5
        
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
            prevphero = self.getPhero(e[0],e[1])
            nextphero = (1 - self.evaprate)*prevphero
            self.setPhero(e[0],e[1],nextphero)
        
    def runColony(self):
        print("run colony ...")
        count = 0
        for i in range(self.maxit):
            bobj = self.bestobj
            self.runIt(i)
            if self.bestobj == bobj:
                count += 1
            else:
                count = 0
            if count >= ceil(log(len(self._g))):
                break            
            
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
    
    def setPriorityPhero(self, i, path, prevphero, obj):
        if self.alg == 'elitist':
            if inPath(path[i],path[i+1],self.bestpath):
                dElit = 1/self.bestobj
                self.setPhero(path[i],path[i+1],
                              prevphero + 1/obj + self.nelit*dElit)
        if self.alg == 'rank':
            for r in range(self.rank):
                if inPath(path[i],path[i+1],self.rankedants[r][0]):
                    dPrio = 1/self.rankedants[r][1]
                    self.setPhero(path[i],path[i+1],
                                  prevphero + (self.rank-r)*dPrio)
        
    def pheroUpdate(self, tid, path):
        "update the pheromone from thread tid path"
        print("update the pheromone from thread {0}".format(tid))
        self.l.acquire()
        print("thread "+str(tid)+" acquired lock for path:")
        print(path)
        self.pheroEvaporate()
        o = obj(self._g,path)
        # select the best path and obj
        if o < self.bestobj:
            self.bestobj = o
            self.bestpath = path
        # add to rankedants if curr obj is smaller than ant with biggest obj
        
        if self.alg == 'rank' and o < self.rankedants[-1][1]:
            for r in range(self.rank):
                if o < self.rankedants[r][1]:
                    self.rankedants[r][0] = path
                    self.rankedants[r][1] = o
                    break
            sorted(self.rankedants, key=lambda ant: ant[1])
        # update pheromones
        for i in range(len(path)-1):
            prevphero = self.getPhero(path[i],path[i+1])
            if self.alg == 'antsys':
                self.setPhero(path[i],path[i+1], prevphero + 1/o)
            else:
                self.setPriorityPhero(i, path, prevphero, o)
        self.l.release()
        print("thread "+str(tid)+" released lock" )
