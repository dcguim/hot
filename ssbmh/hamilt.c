#include<stdio.h>
#include<stdlib.h>

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

void free_path (path * p)
{
  free(p->path);
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
  g->bigM = 0;
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

int main (int argc, char** argv)
{
  graph * g = graph_from_file(argv[1]);  
  path p = traverse(g);  
  printf("path: ");
  for (int i = 0; i < p.length; ++i)
    {
      printf("%d ", p.path[i]);
    }
  int o = cbtsp_o (g, &p);
  printf("costs %d\n", o);
  free_path(&p);
  free_graph(g);
  return 0;
}
