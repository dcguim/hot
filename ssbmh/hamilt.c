#include<stdio.h>
#include<stdlib.h>

typedef struct
{
  int v1,v2;
  int c;
} edge;

typedef struct
{
  int n, m;
  edge * edges;
} graph;

int * traverse (graph * g)
{
  int * path = (int*) malloc ((g->n) * sizeof(int));
  int v = g->edges[0].v1;
  path[0] = v;
  for(int i = 1; i <= g->n; i++)
    {
      int nextn = g->edges[v-1].v2;
      path[i] = nextn;
      v = nextn;
    }
  return path;
}

graph * new_graph (int n, int m)
{
  graph * g = (graph*) malloc(sizeof(graph));
  g->n = n;
  g->m = m;
  g->edges = (edge*) malloc(m * sizeof(edge));
  return g;
}

void free_graph (graph * g)
{
  free(g->edges);
  free(g);
}

graph * graph_from_file (char* file)
{
  int n, m;
  FILE * fs;
  fs = fopen(file, "r");
  fscanf(fs, "%d %d", &n, &m);
  graph * g = new_graph(n, m);
  int i = 0;
  while (fscanf(fs, "%d %d %d\n",
	  &(g->edges[i].v1), &(g->edges[i].v2), &(g->edges[i].c)) != -1)
    {
      i += 1;
    }
  fclose(fs);
  return g;
}

int main (int argc, char** argv)
{
  graph * g = graph_from_file(argv[1]);
  for (int i = 0; i < g->m; i++)
    {
      printf("%d -- %d\n", g->edges[i].v1, g->edges[i].v2);
    }

  int * path = traverse(g);
  for (int i = 0; i <= g->n; i++)
    {
      printf("%d ", path[i]);
    }
  printf("\n");
  free_graph(g);
  free(path);
  return 0;
}
