
#Each ant constructs a solution as follows. First, one of the nodes of the TSP graph is randomly chosen as start node. Then, the ant builds a tour in the TSP graph by moving in each construction step from its current node (i.e., the city in which she is located) to another node which she has not visited yet. At each step the traversed edge is added to the solution under construction. When no unvisited nodes are left the ant closes the tour by moving from her current node to the node in which she started the solution construction. This way of constructing a solution implies that an ant has a memory T to store the already visited nodes. Each solution construction step is performed as follows. Assuming the ant to be in node vi , the subsequent construction step is done with probability
from threading import Thread
from time import sleep
from random import uniform
class Ant(Thread):
    def __init__(self,tid, init_node,init_unvisited,colony):
        Thread.__init__(self)
        self.tid = tid
        self.init = init_node
        self.curr = init_node
        self.unvisit = init_unvisited
        self.n = len(init_unvisited)
        self.__colony = colony
        self.path = []
        self.path.append(init_node)            

    def select_neighbor(self):
        "Assuming alpha and beta are zero"        
        neighs = self.__colony.neighborhood(self.curr)
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
        for i in range(self.n):
            print("thread " + str(self.tid) + " run "+ str(i))
            next_node = self.select_neighbor()
            
