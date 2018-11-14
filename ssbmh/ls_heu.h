typedef struct pair_edge {
  int length;
  edge* e1;
  edge* e2;
}pair_edge;

pair_edge * neighb_str(graph *g, path * p,int * size);

void free_pair_edge(pair_edge* p, int n);
