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
void calc_edge_combinations(edge* available, int availen, pair_edge* result,
			    int r, int index)
{
  for (int i=0;i<availen -1;i++)
    {
      int v1 = available[i].v1.id;
      int v2 = available[i].v2.id;
      int j;
      for (j=i+1;j<availen;j++)
	{
	  result[j-1 + index].e1->v1.id = v1;
	  result[j-1 + index].e1->v2.id = v2;
	  result[j-1 + index].e2->v1.id = available[j].v1.id;
	  result[j-1 + index].e2->v2.id = available[j].v2.id;
	}
      index = j+1;
    }
}
void calc_path_combinations(path* p, pair_edge*  result, int start, int index,
		       int r, int v1, int v2)
{
  if (start > 0)
    {
      for (int i = start; i < p->length-1; i++)
	{
	  result[i-1+index].e1->v1.id = v1;
	  result[i-1+index].e1->v2.id = v2;
	  result[i-1+index].e2->v1.id = p->path[i];
	  result[i-1+index].e2->v2.id = p->path[i+1];	 
	}
      return;
    }
  for (int i=start; i<=p->length-3; i++) 
    { 
      int v1 = p->path[i];
      int v2 = p->path[i+1];
      index = get_index(i,p->length);
      calc_path_combinations(p, result, i+1, index, r, v1, v2); 
    }
  return;
}

// Two is passed as a parameter because these functions are not yet
// generalized to r.
void edge_combinations(edge* available, int ncomb, pair_edge* result, int r)
{
  calc_edge_combinations(available, ncomb, result, 2, 0);
  return;
}
void path_combinations(path* p, pair_edge* comb, int r)
{  
  calc_path_combinations(p, comb, 0,0, 2,-1,-1);
  return;
}
void combine(pair_edge* edge_comb, int encomb, pair_edge* path_comb,
		  int pncomb, pair_edge* result)
{
  int index = 0;
  for (int i=0; i<encomb;i++)
    {
      int j;
      for (j=i; j<pncomb;j++)
	{
	  // Set the pair of edges form the unused edges
	  result[2*j+index].e1->v1.id = edge_comb[i].e1->v1.id;
	  result[2*j+index].e1->v2.id = edge_comb[i].e1->v2.id;
	  result[2*j+index].e2->v1.id = edge_comb[i].e2->v1.id;
	  result[2*j+index].e2->v2.id = edge_comb[i].e2->v2.id;
	  // Set the pair of the edges in the path
	  result[2*j+index+1].e1->v1.id = path_comb[j].e1->v1.id;
	  result[2*j+index+1].e1->v2.id = path_comb[j].e1->v2.id;
	  result[2*j+index+1].e2->v1.id = path_comb[j].e2->v1.id;
	  result[2*j+index+1].e2->v2.id = path_comb[j].e2->v2.id;
	}
      index=2*j+1;
    }
  return;
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

pair_edge * neighb_str(graph *g, path * p,int* size)
{
  int available_len = g->m - p->length+1;
  edge * available = (edge*) malloc(available_len * sizeof(edge));
  // Get all the available edges.
  int j=0;
  for (int i=0; i<g->m;i++)
    {
      if (in_path(g->edges[i],p) == 0)
  	{
  	  available[j].v1.id = g->edges[i].v1.id;
  	  available[j].v2.id = g->edges[i].v2.id;
  	  available[j].c = g->edges[i].c;
  	  j++;
  	}
    }

  // Get all the combinations of available edges
  int edge_ncomb=no_comb(available_len-1);
  pair_edge* edge_comb = new_pair_edge(edge_ncomb);
  edge_combinations(available,available_len,edge_comb,2);

  // Get all the combinations of edges in the path
  int path_ncomb=no_comb(p->length-2);
  pair_edge* path_comb = new_pair_edge(path_ncomb);
  path_combinations(p, path_comb, 2);
  *size = 2*path_ncomb*edge_ncomb;
  pair_edge* comb = new_pair_edge(*size);
  combine(edge_comb,edge_ncomb,path_comb,path_ncomb,comb);  

  free_pair_edge(edge_comb,edge_ncomb);
  free_pair_edge(path_comb,path_ncomb);
  free(available);
  return comb;
}







