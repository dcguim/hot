# Heuristics Optimization Techniques
## Single-solution-based Metaheuristics

Develop single-solution-based metaheuristics for the cost-balanced traveling 
salesperson problem (CBTSP) using varied heuristics:
 - Deterministic and randomized construction heuristics
 - Framework for simple local search, using different neighborhood structures and step functions

###  Modules
#### `hamilt`: Main module which call the heuristics
This module first transform the provided test file into a graph with `graph_from_file`. A graph here is just
implemented as a collection of edges `E` and of vertices `V`, as well as it's sizes, `m = |E|`  and `n = |V|`
graphs also contain a `bigM` which are values that are big enough so to render an edge unpickable in 
minimization problems, in this case it is usually two times the sum of `n` costliest edges.
Additionally, the notion of a `path` is defined as a collection of node ids, the struct also contains
a `length` and the sum of it's edges costs called `distance`. Edges contain two vertices and a cost, 
and vertices contain an id and a degree, i.e. the number of incident edges, for more information 
please look at `hamilt.h`. 

The following algorithms/heuristics are implemented:
- deterministic construction heuristic (`det_ch`)
- randomized construction heuristic (`rand_ch`)
- local search (`ls`)
- greedy randomized adaptive search procedure (`grasp`)
###### structs
- edge
- vertex
- graph
- path
###### functions
- **path * new_path (int length)**: allocates a path of size length and `LLONG_MAX` distance.
- **void free_path (path * p):** frees the allocated path.
- **void path_print(path * p)**: prints the path.
- **void edges_print(graph * g)**: prints the edges in the given graph `g`.
- **int cmp_int (const void * a, const void * b)**: utility to compare int pointers.
- **cost_t cbtsp_o(graph * g, path * p)**: objective function for the cost-balanced tsp
- **cost_t distance(graph * g, int a, int b)**: returns the distance between two ids or bigM if no edge exist
- **double rand_double()**: returns a random double value in the range [0, 1)
- **edge * find_edge (graph * g, int a, int b)**: returns the edge if exists or null
- **path* invert (path* p, int beg, int end)**: invert a section beg-end of a path.
- **int feasible(path * p)**:
- **path * copy_path (path* p)**:
- **path * assign_path (path* a, path* p)**:

#### `const_heu`: Implements construction heuristics
The difference between the nearest neighbor and the nearest neighbor randomized approach is that the
fomer build the hamiltonean path by greedily selecting the node with the shortest distance from the
current while the randomized selects considers `floor(max*r)` closest neighbors and randomly picks one.
###### functions
- **path * ch_nearest_neighbor (graph * g, int start)**: greedly builds a hamiltonean path by picking the closest neighbors
- **path * ch_nearest_neighbor_randomized (graph * g, int start, double r)**: builds a hamiltonean path by picking the neighbors randomly.

##### ls_heu: Implementation of the local search heuristics for 2,3-opt
###### structs: 
- pair_edge
###### functions
 - pair_edge * neighb_str(graph *g, path * p,int * size): Returns a array of pair_edge where the even k-th pair represent the edge in the path and the odds k+1-th pair the edge not in the path that would be substituted by. It takes as input the graph, a solution in form of a path and a pointer to an integer which will contain the size of the neighborhood
 - pair_edge * new_pair_edge(int n): Allocate a new pair edge array of size n
 - void free_pair_edge(pair_edge* p, int n): Free a pair edge array of size n
 - path * replace_edges (path* g, pair_edge * edges, int len): Replace len edges in the path, where edges is an array of pair_edge where the e1 edge are subsituted by the e2.
 - const int in_path(edge e,path * p): Check if an edge is in the given path, if it is returns the index in the path if not -1
 - void neighb_print(pair_edge* neighb, int neighb_len): Print the given neighborhood structure, showing the edges in the path -> edges outside the path which could be used.
##### tabu_heu: Implement simple tabu heuristic with fixed and tabu list, max number of iterations and tenure of half the size of the edges, which runs until no better solution was found in max number of iteration.
###### functions
- path* tabu (graph* g, path* init): Returns a path using tabu heuristics given an initial solution and the graph.
####  Usage
##### Compilation
```sh
$ gcc -Wall hamilt.c const_heu.c ls_heu.c pcg_basic.c grasp.c tabu_heu.c vnd.c -o hamilt
```

with debug output enabled:
```sh
$ gcc -Wall -DPRINTD hamilt.c const_heu.c ls_heu.c pcg_basic.c grasp.c tabu_heu.c vnd.c -o hamilt
```
##### Execution
Simple usage:
```sh
$ ./hamilt ../tests/testname ALGORITHM START_VERTEX_ID
```
Elaborate usage:
```sh
$ ./hamilt ../tests/testname ALGORITHM START_VERTEX_ID NEIGHBORHOOD_METHOD STEP_FN [RUNTIME_SEC]
```
### Notes / TODOs
- The edges are dynamically allocated two times, as graph edges and as the the veritices that are connected to each given vertex. Therefore 2*O(exp(n)) which is not efficient, however we will try to preserve the convenience of having this structure while dynamically allocating it only once by pointing to a unique graph structure. This will be done in the future as a efficiency issue.
- In the ls_heu there are two ways of computing all the combinations of a group of edges, if they are defined by a path or simply as a array of edges, however it may be interesting to change the the struct path to simply an array of edges, instead of storing only the ids, the cost of adapting the code to this mod. is not so big.

