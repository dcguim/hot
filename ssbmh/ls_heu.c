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

void path_combinations(path* p, pair_edge* comb, int r)
{  
  calc_path_combinations(p, comb, 0,0, 2,-1,-1);
  return;
}
int edges_connectedp(edge* e1, edge* e2)
{
  if (e1->v1.id == e2->v1.id || e1->v1.id == e2->v2.id ||
      e1->v2.id == e2->v1.id || e1->v2.id == e2->v2.id)
    {
      return 1;
    }
  return 0;
}
int edges_equal(edge* e1, edge* e2)
{
  if ((e1->v1.id == e2->v1.id &&  e1->v2.id == e2->v2.id) ||
      (e1->v1.id == e2->v2.id &&  e1->v2.id == e2->v1.id))
    return 1;
  return 0;
}
pair_edge* create_neighborhood(graph* g, int* avail, int avail_len, pair_edge* path_comb,int pncomb, int* neigh_len)
{
  // Create a neighborhood array which will represent every pair of edge that has
  // a matching pair of edge from the available edge_comb array which can be switched
  int neighind[pncomb][3];
  for(int i=0;i<pncomb;i++)
    {
      neighind[i][0] = -1;
      neighind[i][1] = -1;
      neighind[i][2] = -1;
      if (edges_connectedp(path_comb[i].e1,path_comb[i].e2) == 0)
	{
	  printf("e1:(%d %d), e2:(%d %d) not connected \n",
		path_comb[i].e1->v1.id,path_comb[i].e1->v2.id,path_comb[i].e2->v1.id,
		path_comb[i].e2->v2.id);
	  // make sure either (path_comb[i].e1.v1, path_comb[i].e2.v2) are in the available
	  // or path_comb[i].e1.v1, path_comb[i].e2.v1 or ... 2 more are in the available
	  int avail1 = -1;
	  int avail2 = -1;
	  for (int j=0;j<avail_len;j++)
	    {
	      edge e;
	      e.v1 = path_comb[i].e1->v1;
	      e.v2 = path_comb[i].e2->v1;
	      if (edges_equal(&(g->edges[avail[j]]), &e) == 1)		   
		{
		  printf("graph[%d]:edge %d %d\n",j,g->edges[avail[j]].v1.id,g->edges[avail[j]].v2.id);
		  avail1 = avail[j];
		  break;
		}		    
	    }
	  for (int j=0;j<avail_len;j++)
	    {
	      edge e;
	      e.v1 = path_comb[i].e1->v2;
	      e.v2 = path_comb[i].e2->v2;
	      if (edges_equal(&(g->edges[avail[j]]), &e) == 1)
		{
		  printf("graph[%d]:edge %d %d\n",j,g->edges[avail[j]].v1.id,g->edges[avail[j]].v2.id);
		  avail2 = avail[j];
		  break;
		}		    
	    }
	  if (avail1 > -1 && avail2>-1)
	    {
	      printf("edges no: %d, %d replaced by: (%d , %d) (%d , %d)\n",
		     avail1,avail2,
		     path_comb[i].e1->v1.id,path_comb[i].e1->v2.id,
		     path_comb[i].e2->v1.id,path_comb[i].e2->v2.id);
	      neighind[i][0] = avail1;
	      neighind[i][1] = avail2;
	      neighind[i][2] = i;
	      
	    }
	}
    }
  int size_neigh = 0;
  for(int i=0;i<pncomb;i++)
    {
      if (neighind[i][0] >-1)
	{
	  neighind[size_neigh][0] = neighind[i][0];
	  neighind[size_neigh][1] = neighind[i][1];
	  neighind[size_neigh][2] = neighind[i][2];
	  size_neigh++;
	}
    }
  *neigh_len = 2*size_neigh;
  pair_edge* neighb = new_pair_edge(*neigh_len);
  int i = 0;
  int j = 0;
  while(neighind[i][0] > -1)
    {
      neighb[j].e1->v1.id = path_comb[neighind[i][2]].e1->v1.id;
      neighb[j].e1->v2.id = path_comb[neighind[i][2]].e1->v2.id;
      neighb[j].e2->v1.id = path_comb[neighind[i][2]].e2->v1.id;
      neighb[j].e2->v2.id = path_comb[neighind[i][2]].e2->v2.id;
      
      neighb[j+1].e1->v1.id = g->edges[neighind[i][0]].v1.id;
      neighb[j+1].e1->v2.id = g->edges[neighind[i][0]].v2.id;
      neighb[j+1].e2->v1.id = g->edges[neighind[i][1]].v1.id;
      neighb[j+1].e2->v2.id = g->edges[neighind[i][1]].v2.id;
      i++;
      j = j + 2;
    }
  return neighb;
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
  //edge * available = (edge*) malloc(available_len * sizeof(edge));
  int available[available_len];
  // Get all the available edges.
  int j=0;
  for (int i=0; i<g->m;i++)
    {
      if (in_path(g->edges[i],p) == 0)
  	{
	  available[j] = i;
  	  j++;
  	}
    }

  // Get all the combinations of edges in the path
  int path_ncomb=no_comb(p->length-2);
  pair_edge* path_comb = new_pair_edge(path_ncomb);
  path_combinations(p, path_comb, 2);

  // Define the neighborhood structure with help of the two arrays of pair edge
  pair_edge* neighb = create_neighborhood(g,available,available_len,path_comb,path_ncomb,size);  

  free_pair_edge(path_comb,path_ncomb);
  return neighb;
}







