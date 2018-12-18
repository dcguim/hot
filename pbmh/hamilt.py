#!/usr/bin/env python3

from sys import argv
from networkx import Graph

def read_instance(filename):
    with open(filename, mode='r') as instance:
        values = [tuple(int(x) for x in line.split()) for line in instance]
        head, edges = values[0], values[1:]
        n, m = head[0], head[1]
        return n, m, edges

class BTSPGraph(Graph):
    def calculate_bigM(self):
        n = len(self)
        edges_weighted = sorted(e[2] for e in self.edges(data='weight'))
        min_sum = sum(edges_weighted[0:n])
        max_sum = sum(edges_weighted[-n:])
        return 2 * max(abs(min_sum), abs(max_sum))

filename = argv[1]
n, m, edges = read_instance(filename)
g = BTSPGraph()
g.add_weighted_edges_from(edges)
g.bigM = g.calculate_bigM()
print("n={} m={} M={}".format(len(g), g.size(), g.bigM))
