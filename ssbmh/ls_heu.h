typedef struct pair_edge {
  edge* e1;
  edge* e2;
}pair_edge;

pair_edge * new_pair_edge(int n);

pair_edge * neighb_str(graph *g,  path * p,int * size_neigh);

void free_pair_edge(pair_edge* p, int n);

path* ls_best_improv (graph* g, path* p);

void replace_edges (path* r, path* p, pair_edge* edges, int i);

const int in_path(edge e,path * p);
