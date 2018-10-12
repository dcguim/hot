#include<stdio.h>
#include<stdlib.h>
typedef struct
{
  int v1,v2;
  int c;
} edge;
int* traverse(edge* graph, int sn)
{
  int v = graph[0].v1;
  int* path;
  path = (int*) malloc ((sn)*sizeof(int));
  path[0] = v;
  for(int i=1; i<= sn; i++)
    {
      int nextn = graph[v-1].v2;
      path[i] = nextn;
      v = nextn;
    }
  return path;
}

int main (int argc, char** argv)
{
  int * nodes;
  edge * graph;
  int sn;
  int se;
  FILE * fs;
  fs = fopen(argv[1],"r");
  printf("%s\n", argv[0]);
  fscanf(fs, "%d %d", &sn, &se);
  int i = 0;
  graph = (edge*) malloc ((se)*sizeof(edge));
  while (fscanf(fs, "%d %d %d\n", &(graph[i].v1),
		&(graph[i].v2), &(graph[i].c)) != -1)
    {
      printf("%d -- %d\n", graph[i].v1,graph[i].v2);
      i+=1;
    }
  int * path = traverse(graph, sn);
  for (i=0;i<=sn;i++)
    {
      printf("%d ",path[i]);
    }
  printf("\n");
  free(graph);
  free(path);
  printf("%d %d\n", sn, se);
  fclose(fs);
  return 0;
}

