#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct edge;
typedef struct vertex
{
  int id;
  int degree;
  struct edge * edges;
} vertex;

typedef struct edge
{
  vertex v1, v2;
  int c;
} edge;

typedef struct graph
{
  int n, m, bigM;
  edge * edges;
  vertex * vertices;
} graph;

typedef struct path
{
  int * path;
  int length;
} path;

typedef struct edge_diff
{
  edge * e;
  int distance;
  int i;
} edge_diff;

int cmp_int (const void * a, const void * b)
{
  int ai = *(int*) a;
  int bi = *(int*) b;
  if (ai < bi) return -1;
  if (ai > bi) return 1;
  return 0;
}

int cmp_cost (const void * a, const void * b)
{
  int ca = abs(((edge*) a)->c);
  int cb = abs(((edge*) b)->c);
  if (ca < cb) return -1;
  if (ca > cb) return 1;
  return 0;
}

int edge_diff_cmp (const void * a, const void * b)
{
  edge_diff * ad = (edge_diff*) a;
  edge_diff * bd = (edge_diff*) b;
  if (ad->distance < bd->distance) return -1;
  if (ad->distance > bd->distance) return 1;
  return 0;
}

// Returns array of indices of edges in the adj list of the given vertex [v]
// sorted from 'best' to 'worst', where 'best' means the edge's cost added
// to the given [cost] is closest to zero.
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

path * new_path (int length)
{
  path * p = (path*) malloc(sizeof(path));
  p->length = length;
  p->path = (int*) malloc(length * sizeof(int));
  return p;
}

void free_path (path * p)
{
  free(p->path);
  free(p);
}

void path_print(path * p)
{
  for (int i = 0; i < p->length; ++i)
    {
      printf("%d ", p->path[i]);
    }
  printf("\n");
}

path traverse (graph * g)
{
  path p;
  p.path = (int*) malloc ((g->n) * sizeof(int));
  int v = g->edges[0].v1.id;
  p.path[0] = v;
  for (int i = 1; i <= g->n; i++)
    {
      int nextn = g->edges[v].v2.id;
      p.path[i] = nextn;
      v = nextn;
    }
  p.length = g->n + 1;
  return p;
}

edge * find_edge_from_to (graph * g, int a, int b)
{
  vertex v = g->vertices[a];
  int d = v.degree;
  for (int i = 0; i < d; ++i)
    {
      if (v.edges[i].v2.id == b)
        {
          return v.edges + i;
        }
    }
  return NULL;
}

graph * new_graph (int n, int m)
{
  graph * g = (graph*) malloc(sizeof(graph));
  g->n = n;
  g->m = m;
  g->vertices = (vertex*) malloc(n * sizeof(vertex));
  g->edges = (edge*) malloc(m * sizeof(edge));
  return g;
}

void free_graph (graph * g)
{
  for (int i = 0; i < g->n; ++i)
    {
      free(g->vertices[i].edges);
    }
  free(g->vertices);
  free(g);
}

void graph_read_edges (graph * g, FILE * fs)
{
  int i = 0, k = 0;
  while (k != -1)
    {
      k = fscanf(fs, "%d %d %d\n",
            &(g->edges[i].v1.id), &(g->edges[i].v2.id), &(g->edges[i].c));
      i += 1;
    }
  //TODO calculate M
  g->bigM = 9999999;
}

void graph_init_vertices (graph * g)
{
  int n = g->n, m = g->m;
  int * degrees = (int*) malloc(n * sizeof(int));
  // Inititalize the incident edges of every vertex to zero
  for (int i = 0; i < n; ++i)
    {
      degrees[i] = 0;
    }
  // Set the number of incident edges of every vertex to the degree variable
  for (int i = 0; i < m; ++i)
    {
      degrees[g->edges[i].v1.id] += 1;
      degrees[g->edges[i].v2.id] += 1;
    }

  for (int i = 0; i < n; ++i)
    {
      printf("deg [%d]: %d\n", i,degrees[i]);
    }
  // Create the adj list on the vertices
  vertex * v;
  for (int i = 0; i < n; ++i)
    {
      v = g->vertices + i;
      v->degree = degrees[i];
      v->edges = (edge*) malloc(v->degree * sizeof(edge));
    }
  edge e, t;
  for (int i = 0; i < m; ++i)
    {
      e = g->edges[i];

      v = g->vertices + e.v1.id;
      v->edges[--degrees[e.v1.id]] = e;

      v = g->vertices + e.v2.id;
      t.v1.id = e.v2.id;
      t.v2.id = e.v1.id;
      t.c = e.c;
      v->edges[--degrees[e.v2.id]] = t;
    }
  free(g->edges);
  free(degrees);
}

graph * graph_from_file (char * file)
{
  int n, m;
  FILE * fs;
  fs = fopen(file, "r");
  int i;

  printf("Enter graph from file\n");
  fflush(stdout);
  fscanf(fs, "%d %d", &n, &m);
  graph * g = new_graph(n, m);
  graph_read_edges (g, fs);
  fclose(fs);
  printf("m:%d, n:%d\nedges:\n",g->m, g->n);
  for (i=0; i<g->m;i++)
    {
      printf("(%d %d) c:%d\n", g->edges[i].v1.id, g->edges[i].v2.id,g->edges[i].c);
    }
  graph_init_vertices(g);
  return g;
}

// Objective function for the cost-balanced tsp
int cbtsp_o(graph * g, path * p)
{
  int o = 0;
  int l = p->length - 1;
  edge * e;
  for (int i = 0; i < l; ++i)
    {
      e = find_edge_from_to (g, p->path[i], p->path[i + 1]);
      o += (e == NULL) ? g->bigM : e->c;
    }
  return abs(o);
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
          next_v = g->vertices[v].edges[edge_indices[i++]];
          available_v = (int*) bsearch(&next_v.v2.id, available, available_length, sizeof(int), cmp_int);
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
          for (int i = available_i; i < available_length; ++i)
            {
              available[i] = available[i + 1];
            }
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

int main (int argc, char** argv)
{
  graph * g = graph_from_file(argv[1]);
  path * p = ch_nearest_neighbor(g, atoi(argv[2]));
  int o = cbtsp_o (g, p);
  printf("cost of constructed path: %d\n", o);
  free_path(p);
  free_graph(g);
  return 0;
}
