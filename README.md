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

####  Usage
##### Compilation
```sh
$ gcc -Wall hamilt.c const_heu.c -o hamilt
```
##### Execution
```sh
$ ./hamilt test/testname startnode
```

