typedef struct edge_diff
{
  edge * e;
  int distance;
  int i;
} edge_diff;

int * get_edges_ordered_by_closest_to_zero(vertex * v, int cost);

path * ch_nearest_neighbor (graph * g, int start);

int edge_diff_cmp (const void * a, const void * b);
