from threading import Thread
from time import sleep
from random import uniform, choice

class Ant(Thread):
    def __init__(self,tid,randinit,colony):
        Thread.__init__(self)
        self.tid = tid
        self._colony = colony
        if randinit == 0:
            self.init = 0
            self.curr = 0
        elif randinit == 1:
            self.init = self._colony.pickRandomNode()
            self.curr = self.init
        self.unvisit = self._colony.univisited(self.init)
        self.n = len(self.unvisit)
        self.path = []
        self.path.append(self.curr)
        
    def intensifyEdge(self):
        neighs = self._colony.neighborhood(self.curr,self.path)
        if not neighs:
            return self.unvisit[-1]
        hneighs = list(map(lambda n :
                           self._colony.getPhero(self.curr,n)*
                          (1/self._colony.getWeight(self.curr,n))**self._colony.bet,
                          neighs))
        i = 0
        for i in range(len(neighs)):
            if max(hneighs) == hneighs[i]:
                break
        return neighs[i]
    
    def selectNeighbor(self):
        neighs = self._colony.neighborhood(self.curr,self.path)
        # if there are no neighbors, just a pick next unvisited node
        if not neighs:
            return self.unvisit[-1]
        pneighs = list(map(lambda n :
<<<<<<< HEAD
                           (self._colony.getPhero(self.curr,n)**self._colony.alp)*
                           ((1/self._colony.getWeight(self.curr,n))**self._colony.bet),
=======
                           (self.__colony.getPhero(self.curr,n)**self.__colony.alp)*
                           ((1/abs(self.__colony.getWeight(self.curr,n)))**self.__colony.bet),
>>>>>>> 87030cfbd59f0351deccafd28973543d826b31e0
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
        ##print('thread id:{0} no of unvisiteds {1}'.format(self.tid,self.n))
        for i in range(self.n):            
            #print("thread " + str(self.tid) + " run "+ str(i))
            if self._colony.alg == 'antcolsys':
                rand = uniform(0,1)
                prev = self.curr
                if rand <= self._colony.divint:
                    self.curr = self.intensifyEdge()
                else:
                    self.curr = self.selectNeighbor()
                self._colony.pheroLocUpdate(prev, self.curr)
            else:
                self.curr = self.selectNeighbor()
            self.path.append(self.curr)
            self.unvisit.remove(self.curr)
        self.path.append(self.init)
        self.curr = self.init
        self._colony.antsUpdate(self.path)
        if self._colony.alg != 'antcolsys':
            self._colony.pheroUpdate(self.tid,self.path)
        #print("finish thread {0}".format(self.tid))
