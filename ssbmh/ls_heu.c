#include "hamilt.h"
#include "ls_heu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

pair_edge * new_pair_edge(int n)
{
  pair_edge * p = (pair_edge*) malloc(n*sizeof(pair_edge));
  for (int i = 0; i < n; i++)
    {
       p[i].e1 = (edge*) malloc(sizeof(edge));
       p[i].e2 = (edge*) malloc(sizeof(edge)); 
    }
  return p;
}
void free_pair_edge(pair_edge* p, int n)
{
  for (int i = 0; i < n; i++)
    {
      printf("%d ", i);
      fflush(stdout);
      free(p[i].e1);
      free(p[i].e2);
    }
  free(p);
  return;
}
int no_comb(int p)
{
  if (p==1)
    return 1;
  else
    return p+no_comb(p-1);
}
int get_index(int i,int length)
{
  if (i == 0)
    return 0;
  else
    return length - 3 + get_index(i-1,length-1); 
}
void calc_combinations(path* p, pair_edge*  result, int start, int index,
		       int r, int v1, int v2)
{
  printf("path_combinations\nstart:%d\n",start);
  fflush(stdout);
  if (start > 0)
    {
      for (int i = start; i < p->length-1; i++)
	{
	  printf("on [%d]\n",i-1+index);
	  fflush(stdout);
	  result[i-1+index].e1->v1.id = v1;
	  result[i-1+index].e1->v2.id = v2;
	  result[i-1+index].e2->v1.id = p->path[i];
	  result[i-1+index].e2->v2.id = p->path[i+1];
	  printf("e1: (%d %d), e2 (%d %d)\n\n", v1,v2,p->path[i],p->path[i+1]);
	}
      return;
    }
  for (int i=start; i<=p->length-3; i++) 
    { 
      int v1 = p->path[i];
      int v2 = p->path[i+1];
      index = get_index(i,p->length);
      calc_combinations(p, result, i+1, index, r, v1, v2); 
    }
  return;
}

int path_combinations(path* p, pair_edge* comb, int r)
{
  int ncomb=no_comb(p->length-2);
  printf("n of comb %d\n",ncomb);
  comb = new_pair_edge(ncomb);
  calc_combinations(p, comb, 0,0, 2,-1,-1);
  printf("e1:(%d %d)\n", comb[0].e1->v1.id, comb[0].e1->v2.id);
  printf("e2:(%d %d)\n\n", comb[0].e2->v1.id, comb[0].e2->v2.id);
  free_pair_edge(comb,ncomb);
  fflush(stdout);
  return ncomb;
}

const int in_path(edge e,path * p)
{
  for (int i = 0; i < p->length - 1; i++)
    {
      if (p->path[i] == e.v1.id && p->path[i+1] == e.v2.id)
	{
	  return 1;
	}
    }
  return 0;
}

pair_edge * neighb_str(graph *g, path * p)
{
  int available_len = g->m - p->length;
  edge * available = (edge*) malloc(available_len * sizeof(edge));
  // available edges
  printf("edges not in path:\n");
  printf("number of edges %d\n", g->m);
  int j=0;
  /* for (int i=0; i<g->m;i++) */
  /*   { */
  /*     if (in_path(g->edges[i],p) == 0) */
  /* 	{ */
  /* 	  available[j].v1.id = g->edges[i].v1.id; */
  /* 	  available[j].v2.id = g->edges[i].v2.id; */
  /* 	  available[j].c = g->edges[i].c; */
  /* 	  j++; */
  /* 	}       	   */
  /*   } */
  pair_edge* result;
  int ncomb = path_combinations(p, result, 2);
  /* for (int i=0;i<ncomb;i++) */
  /*   { */
  /*     printf("e1:(%d %d)\n", result[i].e1->v1.id, result[i].e1->v2.id); */
  /*     printf("e2:(%d %d)\n\n", result[i].e2->v1.id, result[i].e2->v2.id); */
  /*   } */
  return NULL;
}







