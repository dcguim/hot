
#Each ant constructs a solution as follows. First, one of the nodes of the TSP graph is randomly chosen as start node. Then, the ant builds a tour in the TSP graph by moving in each construction step from its current node (i.e., the city in which she is located) to another node which she has not visited yet. At each step the traversed edge is added to the solution under construction. When no unvisited nodes are left the ant closes the tour by moving from her current node to the node in which she started the solution construction. This way of constructing a solution implies that an ant has a memory T to store the already visited nodes. Each solution construction step is performed as follows. Assuming the ant to be in node vi , the subsequent construction step is done with probability
from threading import Thread
from time import sleep
from random import uniform, choice

class Ant(Thread):
    def __init__(self,tid,randinit,colony):
        Thread.__init__(self)
        self.tid = tid
        self.__colony = colony
        if randinit == 0:
            self.init = 0
            self.curr = 0
        elif randinit == 1:
            self.init = random.choice(self.__colony.__g.nodes)
            self.curr = self.init
        print('Ant init at {0}'.format(self.init))
        self.unvisit = self.__colony.univisited(self.init)
        self.n = len(self.unvisit)
        self.path = []
        self.path.append(self.curr)            
        
    def select_neighbor(self):
        "Assuming alpha and beta are one"
        print('select neigh')
        neighs = self.__colony.neighborhood(self.curr,self.path)
        print(neighs)
        pneighs = list(map(lambda n :
                           self.__colony.getPhero(self.curr,n)*
                           (1/self.__colony.getWeight(self.curr,n)),
                           neighs))
        pneighs = list(map(lambda n: n/sum(pneighs), pneighs))        
        intervals = []
        last = 0
        for i in range(len(pneighs)):
            intervals.append((last,last+pneighs[i]))
            last = last + pneighs[i]
        rand = uniform(0,1)
        print(rand, intervals)
        i = 0
        for i in range(len(intervals)):
            if rand >= intervals[i][0] and rand <= intervals[i][1]:
                break
        print(neighs[i])
        return neighs[i]    
    
    def run(self):
        print('no of unvisiteds {0}'.format(self.n))
        for i in range(self.n):
            print("thread " + str(self.tid) + " run "+ str(i))
            self.curr = self.select_neighbor()
            self.path.append(self.curr)
        self.path.append(self.init)
        self.curr = self.init
        self.__colony.pheroUpdate(self.tid,self.path)
