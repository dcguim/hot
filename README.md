# Heuristics Optimization Techniques Assignment
## Single-solution-based Metaheuristics

Develop single-solution-based metaheuristics for the cost-balanced traveling salesperson problem using varied heuristics: 
 - Deterministic and randomized construction heuristics
 - Framework for simple local search, using different neighborhood structures and step functions

####  Modules
##### hamilt: Main module which call the heuristics, define the graph and run the tests using utils.   
###### structs: 
- edge, vertex, graph and path.
###### functions
- path * new_path (int length)
- void path_print(path * p)
- int cmp_int (const void * a, const void * b)
- int cmp_cost (const void * a, const void * b)
- int cbtsp_o(graph * g, path * p):
Objective function for the cost-balanced tsp
- int feasible (path * p):
Returns -2 if the path is feasible, -1 if the path does not return to the start vertex, or the index of the vertex (different then the starting vertex) if is visited more then once.

##### const_heu: Implementation of the deterministic construction heuristics
###### structs: 
- edge_diff
###### functions
 - int * get_edges_ordered_by_closest_to_zero(vertex * v, int cost): 
Returns array of indices of edges in the adj list of the given vertex [v]
sorted from 'best' to 'worst', where 'best' means the edge's cost added
to the given [cost] is closest to zero.

- path * ch_nearest_neighbor (graph * g, int start):
- int edge_diff_cmp (const void * a, const void * b):

##### ls_heu: Implementation of the local search heuristics for 2,3-opt
###### structs: 
- pair_edge
###### functions
 - edge * neighb_str(graph *g, path * p):
 Returns a array of pair_edge where the even k-th pair represent the edge in the path and the odd k+1-th pair the edge not in the path that would be substituted.

####  Usage
##### Compilation
```sh
$ gcc -Wall hamilt.c const_heu.c ls_heu.c pcg_basic.c -o hamilt
```

with debug output enabled:
```sh
$ gcc -Wall -DPRINTD hamilt.c const_heu.c ls_heu.c pcg_basic.c -o hamilt
```
##### Execution
```sh
$ ./hamilt tests/testname startnode
```
### Notes / TODOs
- The edges are dynamically allocated two times, as graph edges and as the the veritices that are connected to each given vertex. Therefore 2*O(exp(n)) which is not efficient, however we will try to preserve the convenience of having this structure while dynamically allocating it only once by pointing to a unique graph structure. This will be done in the future as a efficiency issue.
- In the ls_heu there are two ways of computing all the combinations of a group of edges, if they are defined by a path or simply as a array of edges, however it may be interesting to change the the struct path to simply an array of edges, instead of storing only the ids, the cost of adapting the code to this mod. is not so big.

