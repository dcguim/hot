#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hamilt.h"
#include "const_heu.h"

typedef struct edge_diff
{
  edge * e;
  int distance;
  int i;
} edge_diff;

int edge_diff_cmp (const void * a, const void * b)
{
  edge_diff * ad = (edge_diff*) a;
  edge_diff * bd = (edge_diff*) b;
  if (ad->distance < bd->distance) return -1;
  if (ad->distance > bd->distance) return 1;
  return 0;
}

edge ** get_edges_ordered_by_distance (vertex * v, int cost)
{
  edge ** sorted_edges = (edge**) malloc(v->degree * sizeof(edge*));
  edge_diff * diff = (edge_diff*) malloc(v->degree * sizeof(edge_diff));
  for (int i = 0; i < v->degree; ++i)
    {
      diff[i].e = v->edges + i;
      diff[i].distance = labs(v->edges[i].c + cost);
    }
  qsort(diff, v->degree, sizeof(edge_diff), edge_diff_cmp);
  for (int i = 0; i < v->degree; ++i)
  {
    sorted_edges[i] = diff[i].e;
  }
  free(diff);
  return sorted_edges;
}

void filter_available_edges (edge ** edges, int * edges_len, int * available,
    int available_len)
{
  int j = 0;
  int len = *edges_len;
  int * available_vertex_id;
  for (int i = 0; i < len; ++i)
    {
      available_vertex_id = (int*) bsearch(&edges[i]->v2.id, available,
	  available_len, sizeof(int), cmp_int);
      if (available_vertex_id != NULL)
	{
	  edges[j++] = edges[i];
	}
    }
  *edges_len = j;
}

void remove_available_vertex (int vertex_id, int * available, int * available_len)
{
  int len = *available_len;
  int * available_vertex_id = (int*) bsearch(&vertex_id, available, len,
      sizeof(int), cmp_int);
  int available_i = available_vertex_id - available;

  // Overwrite the available_i pos of the array available 
  for (int i = available_i; i < len; ++i)
    {
      available[i] = available[i + 1];
    }
  *available_len -= 1;
}

int choose_candidate_list_len (int max, double r)
{
  if (r <= 0)
    {
      return 1;
    }
  else if (r >= 1)
    {
      return max;
    }
  else
    {
      return floor(max * r);
    }
}

path * ch_nearest_neighbor_randomized (graph * g, int start, double r)
{
  // Initialize array of vertex ids to store which vertices are not part of
  // the path and still available. The array is sorted and used with bsearch later.
  int available_len = g->n - 1;
  int * available = (int*) malloc(available_len * sizeof(int));
  for (int i = 0; i < available_len; ++i)
    {
      // Add id, but skip the start vertex
      available[i] = (i < start) ? i : i + 1;
    }

  int path_i = 0;
  int v = start;
  cost_t c = 0;
  path * p = new_path(g->n + 1);
  p->path[path_i++] = v;
  do
    {
      // Find the best vertex that is still available
      int edges_len = g->vertices[v].degree;
      edge ** edges = get_edges_ordered_by_distance(&g->vertices[v], c);
      filter_available_edges(edges, &edges_len, available, available_len);

      if (edges_len == 0)
        {
          // Choose any available vertex
          c += g->bigM;
          p->path[path_i++] = available[--available_len];
        }
      else
        {
          // Choose vertex from the restricted candidate list
	  int candidate_list_len = choose_candidate_list_len(edges_len, r);
	  int next_edge_index = candidate_list_len == 1 ? 0
	    : floor(candidate_list_len * rand_double());

	  // Add next edge to path
	  edge * next_edge = edges[next_edge_index];
	  c += next_edge->c;
          v = next_edge->v2.id;
          p->path[path_i++] = v;

	  // Remove next vertex from list of available vertices
	  remove_available_vertex(v, available, &available_len);
        }
      free(edges);
    }
  while (available_len > 0);
  p->distance = c + distance(g, p->path[path_i - 1], start);
  p->path[path_i++] = start;
  free(available);
  return p;
}

path * ch_nearest_neighbor (graph * g, int start)
{
  return ch_nearest_neighbor_randomized (g, start, 0);
}
