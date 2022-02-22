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

The following algorithms/heuristics could be used:
- deterministic construction heuristic (`det_ch`)
- randomized construction heuristic (`rand_ch`)
- local search (`ls`)
- greedy randomized adaptive search procedure (`grasp`)

Notice their implementation exist in the correspondent modules. The `hamilt.c` also offers a series
of auxiliary functions to operate the structs.
###### structs
- edge
- vertex
- graph
- path
###### functions
- **path * new_path (int length)**: allocates a path of size length and `LLONG_MAX` distance.
- **void free_path (path * p):** frees the allocated path.
- **void path_print (path * p)**: prints the path.
- **void edges_print (graph * g)**: prints the edges in the given graph `g`.
- **int cmp_int (const void * a, const void * b)**: utility to compare int pointers.
- **cost_t cbtsp_o (graph * g, path * p)**: objective function for the cost-balanced tsp
- **cost_t distance (graph * g, int a, int b)**: returns the distance between two ids or bigM if no edge exist
- **double rand_double ()**: returns a random double value in the range [0, 1)
- **edge * find_edge (graph * g, int a, int b)**: returns the edge if exists or null
- **path * invert (path * p, int beg, int end)**: invert a section beg-end of a path.
- **int feasible(path * p)**: returns -1 if the initial and last node ids are the same, or the id of the node contained twice, otherwise -2 if not a hamiltonean path
- **path * copy_path (path * p)**: returns a copy of the path `p`.
- **path * assign_path (path * a, path* p)**: assigns the path->path of a to the path->path of b.

#### `const_heu`: Implements construction heuristics
The difference between the nearest neighbor and the nearest neighbor randomized approach is that the
fomer build the hamiltonean path by greedily selecting the node with the shortest distance from the
current while the randomized selects considers `floor(max*r)` closest neighbors and randomly picks one.
The randomization factor `r` is a value from 0 to 1, the bigger this factor the bigger the amount of
incident nodes it will consider, for `r=1` it will pick randomly any of the of current node's neighbors.
Conversely, notice the following relationship applies:
`ch_nearest_neighbor_randomized (g, start, 0) == ch_nearest_neighbor (g, start)`

###### functions
- **path * ch_nearest_neighbor (graph * g, int start)**: greedly builds a hamiltonean path by picking the closest neighbors
- **path * ch_nearest_neighbor_randomized (graph * g, int start, double r)**: builds a hamiltonean path by picking the neighbors randomly

##### ls_heu: Implementation of the local search heuristics for 2,2.5, and 3-opt

###### structs: 
- pair_edge
###### functions
 - **pair_edge * new_pair_edge (int n)**: Allocate a new pair of edges array of size n
 - **void free_pair_edge (pair_edge * p, int n)**: free the pair of edges
 - **void replace_edges (path * r, path * p, pair_edge * edges, int len);**: replace len edges in the path, where edges is an array of pair_edge where the e1 edge are subsituted by the e2.
 - **const int in_path (edge e, path * p)**: checks if an edge is in the given path, 
 if it is returns the index in the path if not -1 
 - **pair_edge * neighb_str(graph * g, path * p, int * size)**: returns the neighbor structure array of pair_edge with double the size parameter where the even k-th pair 
 represent the edge in the path and the odds k+1-th pair the edge not in the path that would be substituted by.
 It takes as input the graph, a solution in form of a path and a pointer to an integer which will contain the size of the neighborhood
 - **void neighb_print(pair_edge * neighb, int neighb_len)**: print the given neighborhood structure, showing the edges in the path -> edges outside the path which could be used.
 - **path * ls_best_improv (graph * g, path * p, pair_edge * restr, int restr_len, pair_edge * replaced_pair_edge)**: for each consecutive edges in the neighbor structure replace the 
 edges and update the base cost if a smaller path is created.
 - **path * fix_point_ls_best_improv (graph * g, path * init_p)**: runs the previous mentioned algorithm until the path doesn't change for the new neighbor structure provided.
 - **n_3opt_it * n_3opt_new_it ()**: creates an 3-opt iterator, that is a moving 3-index (i,j,k) that points to edges to be replaced in a path
 - **n_25opt_it * n_25opt_new_it ()**: creates an 2.5-opt iterator, that is a moving 3-index (i,j,s) that points to edges to be replaced in a path
 - **path * n_3opt_next (graph * g, path * p, void * it)**: this method replaces the edges incident of nodes (i,j,k) in 4 possible ways without creating loops.
 It replaces the edges by reverting the path in specific subsequences of the path, which is equivalent. It searches for paths shorter than p, until time exceeds
 or the last iterator is bigger or equal the path length - 5 in which case it makes no sense to update the (i,j,k) values.
 - **path * n_3opt_rand (graph * g, path * p, void * it)**: this function simply randomizes the (i,j,k) values while preserving the relationship i<j<k.
 - **path * n_25opt_next (graph * g, path * p, void * it)**: this method first searches for a pair of unexisting edges both to each other, i.e. edges
 with distance equals bigM with the iterator i, and then exchange those edges with another edge pointed by j to replace with, potentially reducing
 the distance to the path. The 2.5-opt is between 2 and 3 opt in the sense that it replaces 3 edges but two of those edges are next to each-other
 the iterator s won't iterate throughout the path, like i and j.
 - **path * single_step (graph * g, path * p, neighborhood_fn n_next, void * it)**: this function returns the candidate path after a single iteration
 of the step function provided, n_next.
 - **path * rand_step (graph * g, path * p, neighborhood_fn n_next, void * it)**: thi function returns the candidate path after multiple iterations
 of the step function provided, each step function has it's own defition of done, but usually it means that the iterators exhausted the search space
 of paths to consider. At each iteration the candidate is added to an array of paths, on the end of the execution of the steps, the algorithm will
 randomize the position of the paths, and returns the first candidate path which has a cost smaller than the initial path p.
 - **path * first_improv (graph * g, path * p, neighborhood_fn n_next, void * it)**: this function steps until the first candidate path which
 has the cost smaller than the initial path is found.
 - **path * best_improv (graph * g, path * p, neighborhood_fn n_next, void * it)**: this function exhausts the seach space generated by the iterators
 and overwrites the best_candidate when a candidate path with smaller cost than the previous one is found.
 - **path * local_search(graph * g, path * p, step_fn step, neighborhood_fn n_next, new_it_fn new_it, double runtime_seconds)**: this function runs
 the main loop in the local search algorithm, it runs one of the previously described step functions until either the runtime given expires or the
 max attempt of fails is reached, i.e. if the candidate generated by running the step function once is not better than the best candidate so far.
 The max number of fails is set to be the log of the size of the path.
 
 
 
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
For `ALGORITHM` = `ch_rand`:
```sh
$ ./hamilt ../tests/testname det_ch START_VERTEX_ID
```
For `ALGORITHM` = `ch_rand` a `RANDOMIZATION FACTOR` from [0..1] is compulsory:
```sh
$ ./hamilt ../tests/testname ch_rand [RANDOMIZATION FACTOR] START_VERTEX_ID
```
For `ALGORITHM` = `ls`, 
`NEIGHBORHOOD_METHOD` in [`2opt`, `2.5opt`, `3opt`], and
`STEP_FN` in [`first_improv`, `best_improv`, `rand`]
```sh
$ ./hamilt ../tests/testname ls START_VERTEX_ID NEIGHBORHOOD_METHOD STEP_FN [RUNTIME_SEC]
```
### Notes / TODOs
- The edges are dynamically allocated two times, as graph edges and as the the veritices that are connected to each given vertex. Therefore 2*O(exp(n)) which is not efficient, however we will try to preserve the convenience of having this structure while dynamically allocating it only once by pointing to a unique graph structure. This will be done in the future as a efficiency issue.
- In the ls_heu there are two ways of computing all the combinations of a group of edges, if they are defined by a path or simply as a array of edges, however it may be interesting to change the the struct path to simply an array of edges, instead of storing only the ids, the cost of adapting the code to this mod. is not so big.
- Rewrite the n_25opt_next.
- Attempt to integrate the 2opt as 25opt and 3opt using an iterator function.
- Implement the first improv and and random improvement for functionalities for 2opt
