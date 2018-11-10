#include <stdlib.h>
#include <math.h>
#include "hamilt.h"
#include "const_heu.h"

int edge_diff_cmp (const void * a, const void * b)
{
  edge_diff * ad = (edge_diff*) a;
  edge_diff * bd = (edge_diff*) b;
  if (ad->distance < bd->distance) return -1;
  if (ad->distance > bd->distance) return 1;
  return 0;
}

path * ch_nearest_neighbor (graph * g, int start)
{
  // Initialize array of vertex ids to store which vertices are not part of
  // the path and still available. The array is sorted and used with bsearch later.
  int available_length = g->n - 1;
  int * available = (int*) malloc(available_length * sizeof(int));
  for (int i = 0; i < available_length; ++i)
    {
      // Add id, but skip the start vertex
      available[i] = (i < start) ? i : i + 1;
    }

  int path_i = 0;
  int v = start;
  int c = 0;
  path * p = new_path(g->n + 1);
  p->path[path_i++] = v;
  do
    {
      // Find the best vertex that is still available
      int i = 0;
      edge next_v;
      int * available_v;
      int * edge_indices = get_edges_ordered_by_closest_to_zero(
          &g->vertices[v], c);
      do
        {
	  // available_v is a pointer to the the vertices connected to v in
	  // the available array
          next_v = g->vertices[v].edges[edge_indices[i++]];
          available_v = (int*) bsearch(&next_v.v2.id, available,
				       available_length, sizeof(int),
				       cmp_int);	  
        }
      while (i < g->vertices[v].degree && available_v == NULL);
      if (available_v == NULL)
        {
          // Choose any available vertex
          c += g->bigM;
          p->path[path_i++] = available[--available_length];
        }
      else
        {
          // Choose best vertex and remove it from list of available vertices
          int available_i = available_v - available;
          available_length -= 1;
	  // Overwrite the available_i pos of the array available 
          for (int i = available_i; i < available_length; ++i)
            {
              available[i] = available[i + 1];
            }
	  // Next vector becomes the next_v
          c += next_v.c;
          v = next_v.v2.id;
          p->path[path_i++] = v;
        }
      free(edge_indices);
    }
  while (available_length > 0);
  p->path[path_i++] = start;
  free(available);
  return p;
}

int * get_edges_ordered_by_closest_to_zero(vertex * v, int cost)
{
  int * distances = (int*) malloc(v->degree * sizeof(int));
  edge_diff * diff = (edge_diff*) malloc(v->degree * sizeof(edge_diff));
  for (int i = 0; i < v->degree; ++i)
    {
      diff[i].e = &v->edges[i];
      diff[i].distance = abs(v->edges[i].c + cost);
      diff[i].i = i;
    }
  qsort(diff, v->degree, sizeof(edge_diff), edge_diff_cmp);
  for (int i = 0; i < v->degree; ++i)
  {
    distances[i] = diff[i].i;
  }
  free(diff);
  return distances;
}
