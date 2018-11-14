#include <stdio.h>
#include <stdlib.h>
#include "hamilt.h"
#include "const_heu.h"
#include "ls_heu.h"

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

void edges_print(graph * g)
{
  printf("m:%d, n:%d\nedges:\n",g->m, g->n);
  for (int i=0; i<g->m;i++)
    {
      
      printf("[%d] (%d %d) c:%d\n",i, g->edges[i].v1.id,
	     g->edges[i].v2.id,g->edges[i].c);
    }
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

int calculate_bigm(graph * g)
{
  int bigM = 0;
  for (int i=0; i<g->m;i++)
    {
      bigM += abs(g->edges[i].c);
    }
  return bigM+1;
}


void graph_read_edges (graph * g, FILE * fs)
{
  printf("graph read edges\n");
  int i = 0, k = 0;
  while (k != -1)
    {
      k = fscanf(fs, "%d %d %d\n",
            &(g->edges[i].v1.id), &(g->edges[i].v2.id), &(g->edges[i].c));
      printf("%d (%d %d)",i,g->edges[i].v1.id, g->edges[i].v2.id);
      i += 1;
    }
  g->bigM = calculate_bigm(g);
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
  free(degrees);
}

graph * graph_from_file (char * file)
{
  int n, m;
  FILE * fs;
  fs = fopen(file, "r");
  fscanf(fs, "%d %d", &n, &m);
  graph * g = new_graph(n, m);
  graph_read_edges (g, fs);
  fclose(fs);  
  graph_init_vertices(g);
  return g;
}

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

int feasible(path * p)
{
  if (p->path[0] != p->path[p->length])
    {
      return -1;
    }
  for (int i = 0; i < p->length; i++)
    {
      int v = p->path[i];
      for (int j = i+1; j < p->length; j++)	
	{
	  if (p->path[i] == v)
	    {
	      return v;
	    }
	}
    }
  return -2;
}


int main (int argc, char** argv)
{
  graph * g = graph_from_file(argv[1]);
  edges_print(g);  
  path *  pch = ch_nearest_neighbor(g, atoi(argv[2])); 
  path_print(pch); 
  int o = cbtsp_o (g, pch);
  printf("nearest neighbor cost of constructed path: %d\n", o);
  int neighb_len = 0;
  pair_edge * neighb = neighb_str(g, pch, &neighb_len);
  printf("neighborhood of size: %d\n",neighb_len);
  free_pair_edge(neighb, neighb_len);
  free_path(pch);
  free_graph(g);
  return 0;
}
