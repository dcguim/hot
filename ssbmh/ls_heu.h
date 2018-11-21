typedef struct pair_edge {
  edge* e1;
  edge* e2;
}pair_edge;

typedef struct n_3opt_it {
  int i, j, k;
} n_3opt_it;

typedef path * (*neighborhood_fn)(graph * g, path * p, void* it);

typedef path * (*step_fn)(graph * g, path * p, neighborhood_fn n_next, void* it);

pair_edge * new_pair_edge(int n);

pair_edge * neighb_str(graph *g,  path * p,int * size_neigh);

void free_pair_edge(pair_edge* p, int n);

path* ls_best_improv (graph* g, path* p);

void replace_edges (path* r, path* p, pair_edge* edges, int i);

const int in_path(edge e,path * p);

path * local_search(graph * g, path * p, step_fn step, neighborhood_fn n_next, void* it, double runtime_seconds);

n_3opt_it n_3opt_new_it ();

path * n_3opt_next (graph *g, path * p, void* it);

path * first_improv (graph * g, path * p, neighborhood_fn n_next, void* it);
