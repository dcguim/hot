import math
from itertools import combinations

from hamilt import objsum, obj
from colony import Colony

class HybridColony(Colony):
    def __init__(self,alg,nants,graph,max_iterations,evap_rate,rand_init,
                 # optional params
                 nelit,alph,beta,rank,ls,abhc):
        self.__oldbestpath = None
        self.do_ls = ls
        self.do_abhc = abhc
        super().__init__(alg,nants,graph,max_iterations,evap_rate,rand_init,
                nelit,alph,beta,rank)

    def runColony(self):
        super().runColony()
        print(' '.join(map(str, self.bestpath[1:] + [self.bestobj])))

    def runIt(self, i):
        super().runIt(i)
        new_bestpath = self.__oldbestpath is not self.bestpath
        path = Path(self._g, self.bestpath)
        #ph = dict(self._pheromone)

        # local search
        if self.do_ls:
            improved = False
            while new_bestpath:
                neighborhood = TwoOptNeighbors(self._g, path, aspiration=False)
                best_path, edges = neighborhood.get_best_improv()
                if best_path is not None and best_path.obj < self.bestobj:
                    self.bestpath = best_path.path
                    self.bestobj = best_path.obj
                    path = best_path
                    improved = True
                    print('LS  {}'.format(path.obj))
                else:
                    break
            if improved:
                self.__update_ph(path)

        # attribute based hill climber
        if self.do_abhc:
            improved = False
            while new_bestpath:
                neighborhood = TwoOptNeighbors(self._g, path)
                best_path, edges = neighborhood.get_best_improv()
                #assert best_path == None or best_path.obj == obj(self._g, best_path.path)
                if best_path is not None and best_path.obj < self.bestobj:
                    #print('replaced \n{} ({})\nwith\n{} ({})'.format(
                    #    self.bestpath, obj(self._g, self.bestpath),
                    #    best_path.path, obj(self._g, best_path.path)))
                    self.__update(path, (best_path, edges), ph=False)
                    self.bestpath = best_path.path
                    self.bestobj = best_path.obj
                    path = best_path
                    improved = True
                    print('BHC {}'.format(path.obj))
                else:
                    break
            if improved:
                self.__update_ph(path)
                self.__update_ph_by_value()

        self.__oldbestpath = self.bestpath

        #d = {k: self._pheromone[k] - ph.get(k, 0) for k in
        #        self._pheromone.keys() if self._pheromone[k] - ph.get(k, 0) > 0}
        #if len(d) > 0:
        #    print(d)

    def __update(self, path, neighbor, val=True, ph=True):
        best_path, edges = neighbor
        p = path.path
        a = (p[edges[0]], p[edges[1]])
        b = (p[edges[0]+1], p[edges[1]+1])
        if best_path.obj >= path.obj and not ph:
            if self._g.has_edge(*a):
                a_value = self._g[a[0]][a[1]].get('value', math.inf)
                new_a_value = min(a_value, best_path.obj)
                self._g[a[0]][a[1]]['value'] = min(new_a_value, best_path.obj)
                if ph:
                    old_phero = self.getPhero(*a)
                    self.setPhero(*a, old_phero + 1/new_a_value)
            if self._g.has_edge(*b):
                b_value = self._g[b[0]][b[1]].get('value', math.inf)
                new_b_value = min(b_value, best_path.obj)
                self._g[b[0]][b[1]]['value'] = min(new_b_value, best_path.obj)
                if ph:
                    old_phero = self.getPhero(*b)
                    self.setPhero(*b, old_phero + 1/new_b_value)
        else:
            for i in range(len(best_path.path) - 1):
                e = (best_path.path[i], best_path.path[i+1])
                if self._g.has_edge(*e):
                    value = self._g.edges[e].get('value', math.inf)
                    new_value = min(value, best_path.obj)
                    self._g.edges[e]['value'] = best_path.obj
                    if ph:
                        old_phero = self.getPhero(*e)
                        self.setPhero(*e, old_phero + 1/new_value)

    def __update_ph(self, best_path):
        p = best_path.path
        for i in range(len(best_path.path) - 1):
            e = (p[i], p[i+1])
            if self._g.has_edge(*e):
                old_phero = self.getPhero(*e)
                self.setPhero(*e, old_phero + 1/best_path.obj*self.size)

    def __update_ph_by_value(self):
        for (v, w, e) in self._g.edges.data('value', default=math.inf):
            old_phero = self.getPhero(v, w)
            self.setPhero(v, w, old_phero + 1/e)

class Path:
    def __init__(self, graph, path, osum=None):
        self.path = path
        self.osum = objsum(graph, path) if osum is None else osum
        self.obj = abs(self.osum)

class TwoOptNeighbors:
    def __init__(self, graph, path, aspiration=True):
        self.nr_nodes = len(graph)
        self.graph = graph
        self.path = path
        self.aspiration = aspiration
        if aspiration:
            self.worst_edges = self.__get_worst()

    def get_best_improv(self):
        best_objective_sum = math.inf
        best_objective_edges = None
        n_neighbors = 0
        for (edges, osum) in self.__neighbors():
            n_neighbors += 1
            if abs(best_objective_sum) > abs(osum):
                best_objective_sum = osum
                best_objective_edges = edges
        #print('evaluated {} neighbors'.format(n_neighbors))

        if best_objective_edges is None:
            return (None, None)
        best_path = self.__build_neighbor(*best_objective_edges)
        assert best_path[0] == best_path[-1]

        return (Path(self.graph, best_path, osum = best_objective_sum), best_objective_edges)

    def __neighbors(self):
        p = self.path.path
        for (i, j) in self.__combinations():
            a = (p[i], p[i+1])
            b = (p[j], p[j+1])
            osum = self.__calculate_osum(a, b)
            if not self.aspiration or self.__acceptable(a, b, abs(osum)):
                yield ((i, j), osum)

    def __combinations(self):
        for i in range(self.nr_nodes - 2):
            for j in range(i + 2, self.nr_nodes):
                yield (i, j)

    def __calculate_osum(self, a, b):
        return (self.path.osum - self.__get_weight(*a) - self.__get_weight(*b)
                + self.__get_weight(a[0], b[0]) + self.__get_weight(a[1], b[1]))

    def __get_weight(self, v, w):
        return self.graph.get_edge_data(v, w, default={'weight': self.graph.bigM})['weight']

    def __get_value(self, v, w):
        return self.graph.get_edge_data(v, w, default={'value': 0}).get(
                'value', math.inf)

    def __acceptable(self, a, b, objective):
        if (objective < self.__get_value(a[0], b[0]) or
                objective < self.__get_value(a[1], b[1])):
            return True
        elif objective >= self.path.obj:
            return False
        else:
            new_edges = (a, b)
            for (v, w) in self.worst_edges:
                if objective >= self.__get_value(v, w):
                    return False
                elif (v, w) not in new_edges:
                    return True
        assert False

    def __get_worst(self):
        path = self.path.path[1:]
        edges = sorted(self.graph.edges(path, data='value', default=math.inf),
                key=lambda e: e[2], reverse=True)
        return [(v, w) for (v, w, _) in edges[:3]]

    def __build_neighbor(self, i, j):
        p = self.path.path
        rev_path = p[i+1:j+1]
        rev_path.reverse()
        return p[:i+1] + rev_path + p[j+1:]
