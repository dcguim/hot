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
            self.init = self.__colony.pickRandomNode()
            self.curr = self.init
        self.unvisit = self.__colony.univisited(self.init)
        self.n = len(self.unvisit)
        self.path = []
        self.path.append(self.curr)            
        
    def select_neighbor(self):
        "Assuming alpha and beta are one"
        neighs = self.__colony.neighborhood(self.curr,self.path)
        # if there are no neighbors, just a pick next unvisited node
        if not neighs:
            return self.unvisit[-1]
        pneighs = list(map(lambda n :
                           (self.__colony.getPhero(self.curr,n)**self.__colony.alp)*
                           ((1/abs(self.__colony.getWeight(self.curr,n)))**self.__colony.bet),
                           neighs))
        try:
            pneighs = list(map(lambda n: n/sum(pneighs), pneighs))
        except ZeroDivisionError:
            return choice(neighs)
        intervals = []
        last = 0
        for i in range(len(pneighs)):
            intervals.append((last,last+pneighs[i]))
            last = last + pneighs[i]
        rand = uniform(0,1)
        i = 0
        for i in range(len(intervals)):
            if rand >= intervals[i][0] and rand <= intervals[i][1]:
                break
        return neighs[i]    
    
    def run(self):
        print('thread id:{0} no of unvisiteds {1}'.format(self.tid,self.n))
        for i in range(self.n):            
            print("thread " + str(self.tid) + " run "+ str(i))
            self.curr = self.select_neighbor()
            self.path.append(self.curr)
            self.unvisit.remove(self.curr)
        self.path.append(self.init)
        self.curr = self.init
        self.__colony.pheroUpdate(self.tid,self.path)
        print("finish thread {0}".format(self.tid))
