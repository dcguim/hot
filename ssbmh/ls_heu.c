#include "hamilt.h"
#include "ls_heu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

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
void neighb_print(pair_edge* neighb, int neighb_len)
{
  for (int i=0;i<neighb_len;i++)
    {
      if (i%2 == 0)
	printf("\n");
      else
	printf("-> ");
      printf("(%d %d) , (%d %d) ",neighb[i].e1->v1.id,neighb[i].e1->v2.id,
	     neighb[i].e2->v1.id,neighb[i].e2->v2.id);
    }
  printf("\n");
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

// This function return all 2 combinations of all the edges in the path
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
pair_edge* create_neighborhood(graph* g, int* avail, int avail_len,
			       pair_edge* path_comb,int pncomb, int* neigh_len)
{
  // Create a neighborhood array which will represent every pair of edge that has
  // a matching pair of edge from the available edge_comb array which can be switched
  int **neighind = (int **) malloc (pncomb*sizeof(int*));
  for (int i=0; i<pncomb; i++)
    neighind[i] = (int*) malloc(3*sizeof(int));
  for (int i=0; i<pncomb; i++)
    {
      neighind[i][0] = -1;
      neighind[i][1] = -1;
      neighind[i][2] = -1;
      if (edges_connectedp(path_comb[i].e1,path_comb[i].e2) == 0)
	{
	  int avail1 = -1;
	  int avail2 = -1;
	  for (int j=0;j<avail_len;j++)
	    {
	      edge e;
	      e.v1 = path_comb[i].e1->v1;
	      e.v2 = path_comb[i].e2->v1;
	      if (edges_equal(&(g->edges[avail[j]]), &e) == 1)		   
		{
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
		  avail2 = avail[j];
		  break;
		}		    
	    }
	  if (avail1 > -1 && avail2>-1)
	    {
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
  while(neighind[i][0] > -1 && j < *neigh_len)
    {       
      neighb[j].e1->v1.id = path_comb[neighind[i][2]].e1->v1.id;
      neighb[j].e1->v2.id = path_comb[neighind[i][2]].e1->v2.id;
      neighb[j].e2->v1.id = path_comb[neighind[i][2]].e2->v1.id;
      neighb[j].e2->v2.id = path_comb[neighind[i][2]].e2->v2.id;

      neighb[j+1].e1->v1.id = g->edges[neighind[i][0]].v1.id;
      neighb[j+1].e1->v2.id = g->edges[neighind[i][0]].v2.id;
      neighb[j+1].e2->v1.id = g->edges[neighind[i][1]].v2.id;
      neighb[j+1].e2->v2.id = g->edges[neighind[i][1]].v1.id;
      i++;
      j = j + 2;
    }
  for (int i=0; i<pncomb; i++)
    free(neighind[i]);
  free(neighind);
  return neighb;
}
const int in_path(edge e,path * p)
{
  for (int i = 0; i < p->length - 1; i++)
    {
      if ((p->path[i] == e.v1.id && p->path[i+1] == e.v2.id) ||
	  (p->path[i] == e.v2.id && p->path[i+1] == e.v1.id))
	{
	  return i;
	}
    }
  return -1;
}

pair_edge * neighb_str(graph *g, path * p,int* size)
{
  int nedges = g->m;
  int available[nedges];
  // Get all the available edges.
  int j=0;
  for (int i=0; i<g->m;i++)
    {
      if (in_path(g->edges[i],p) == -1)
  	{
	  available[j] = i;
  	  j++;
  	}
    }
  int available_len = j;
  // Get all the combinations of edges in the path
  int path_ncomb=no_comb(p->length-2);
  pair_edge* path_comb = new_pair_edge(path_ncomb);
  path_combinations(p, path_comb, 2);
  // Define the neighborhood structure with help of the two arrays of pair edge
  pair_edge* neighb = create_neighborhood(g,available,available_len,path_comb,path_ncomb,size);
  free_pair_edge(path_comb,path_ncomb);
  return neighb;
}

void replace_edges (path* r, path* p, pair_edge* edges, int len)
{
  // For the invertion of the interval between the two edges in the path
  int interval[2];
  for (int i=0;i<len;i++)
    {
      int j = in_path(*edges[i].e1,p);
      if (i == 0)
	  interval[i] = j+2;
      else
	interval[i] = j-1;
      if (j >=0)
	{
	  r->path[j] = edges[i].e2->v1.id;
	  r->path[j+1] = edges[i].e2->v2.id;
	}
    }
  r = invert(r, interval[0], interval[1]);
}

path* ls_best_improv (graph* g, path* p)
{
  int j = 0;
  int min_ineigh = -1;
  int basecost = cbtsp_o(g, p);
  int neighb_len = 0;
  pair_edge* neighb = neighb_str(g, p, &neighb_len);
  printf("neighborhood of size: %d\n",neighb_len);

  // Create an auxiliary path that will be used to hold all the paths
  // created by replacing edges
  path* r = copy_path(p);
  path* sol = new_path(p->length);
  pair_edge edges[2];  
  do
    {
      edges[0].e1 = neighb[j].e1;
      edges[0].e2 = neighb[j+1].e1;
      edges[1].e1 = neighb[j].e2;
      edges[1].e2 = neighb[j+1].e2;
      replace_edges (r, p, edges, 2);
      // Check if the cost of the replaced solution is smaller than the
      // given path cost, if it is then store it`s index.      
      int rcost = cbtsp_o(g, r);
      if (rcost < basecost)
	{
	  min_ineigh = j;
	  basecost = rcost;
	  assign_path(sol,r);
	}
      
     
      // Refresh r to be equal to p for the next iteration
      assign_path(r,p); 
      j = j+2;
    }
  while (j < neighb_len);

  // Free the auxiliary variables
  free_path(r);
  free_pair_edge(neighb, neighb_len);
  if (min_ineigh == -1)
    return p;
  else    
    return sol;
}

n_3opt_it * n_3opt_new_it ()
{
  n_3opt_it * it = (n_3opt_it*)malloc(sizeof(n_3opt_it));
  it->i = 0;
  it->j = 2;
  it->k = 4;
  return it;
}

n_25opt_it * n_25opt_new_it ()
{
  n_25opt_it * it = (n_25opt_it*)malloc(sizeof(n_25opt_it));
  it->i = 0;
  it->j = 0;
  it->s = 0;
  return it;
}

// Reverse items in p from i to j inclusive
void path_reverse (path * p, int i, int j)
{
  int tmp;
  if (j < i)
    {
      path_reverse(p, j + 1, i - 1);
      return;
    }
  while (i < j)
  {
    tmp = p->path[i];
    p->path[i] = p->path[j];
    p->path[j] = tmp;
    i += 1;
    j -= 1;
  }
}

// Move items in p in range from i to j inclusive to position t
void path_move (path * p, int i, int j, int t)
{
  assert(i < j);
  assert(t < i);
  int l = j - i + 1;
  int m = i - t;
  int * tmp = (int*)malloc(m * sizeof(int));
  memcpy(tmp, p->path + t, m * sizeof(int));
  memcpy(p->path + t, p->path + i, l * sizeof(int));
  memcpy(p->path + t + l, tmp, m * sizeof(int));
  free(tmp);
}

path * n_3opt_next (graph * g, path * p, void* n_it)
{
  n_3opt_it * it = (n_3opt_it*)n_it;
  path * n = copy_path(p);

  for (;;)
    {
      if (it->i < p->length - 5)
	{
	  if (it->j < p->length - 3)
	    {
	      // don't choose edge to vertex 0 as third when the first edge starts from 0
	      if (it->k < p->length - 1 - (it->i == 0))
		{
		  int v1a = p->path[it->i], v1b = p->path[it->i + 1];
		  int v2a = p->path[it->j], v2b = p->path[it->j + 1];
		  int v3a = p->path[it->k], v3b = p->path[it->k + 1];

		  // cost of edges to remove
		  cost_t d0 = distance(g, v1a, v1b) + distance(g, v2a, v2b) + distance(g, v3a, v3b);

		  cost_t f0 = llabs(n->distance);

		  cost_t d_3a1b = distance(g, v1b, v3a);
		  cost_t d1 = n->distance - d0 + distance(g, v1a, v2a) + d_3a1b + distance(g, v2b, v3b);
		  if (llabs(d1) < f0)
		    {
		      path_reverse(n, it->i + 1, it->j);
		      path_reverse(n, it->j + 1, it->k);
		      n->distance = d1;
		      it->k += 1;
		      return n;
		    }

		  cost_t d_1a2b = distance(g, v1a, v2b);
		  cost_t d2 = n->distance - d0 + d_1a2b + distance(g, v3a, v2a) + distance(g, v1b, v3b);
		  if (llabs(d2) < f0)
		    {
		      path_reverse(n, it->j + 1, it->k);
		      path_reverse(n, it->k + 1, it->i);
		      n->distance = d2;
		      it->k += 1;
		      return n;
		    }

		  cost_t d_2a3b = distance(g, v2a, v3b);
		  cost_t d3 = n->distance - d0 + distance(g, v1a, v3a) + distance(g, v2b, v1b) + d_2a3b;
		  if (llabs(d3) < f0)
		    {
		      path_reverse(n, it->i + 1, it->j);
		      // this needs to go second because afterwards indices in it are invalid
		      path_reverse(n, it->k + 1, it->i);
		      n->distance = d3;
		      it->k += 1;
		      return n;
		    }

		  cost_t d4 = n->distance - d0 + d_1a2b + d_3a1b + d_2a3b;
		  if (llabs(d4) < f0)
		    {
		      path_move(n, it->j + 1, it->k, it->i + 1);
		      n->distance = d4;
		      it->k += 1;
		      return n;
		    }
		  else
		    {
		      it->k += 1;
		    }
		}
	      else
		{
		  it->j += 1;
		  it->k = it->j + 2;
		}
	    }
	  else
	    {
	      it->i += 1;
	      it->j = it->i + 2;
	    }
	}
      else
	{
	  return NULL;
	}
    }
}

path * n_3opt_rand (graph * g, path * p, void* _it)
{
  int i = floor((p->length - 5) * rand_double());
  int j = floor(i + 2 + (p->length - 3 - i - 2) * rand_double());
  int k = floor(j + 2 + (p->length - 1 - (i == 0) - j - 2) * rand_double());
  n_3opt_it it = { .i = i, .j = j, .k = k };
  return n_3opt_next(g, p, &it);
}

path * n_25opt_next (graph * g, path * p, void* n_it)
{
  n_25opt_it * it = (n_25opt_it*)n_it;
  path * n = copy_path(p);

  for (;;)
    {
      if (it->i < p->length - 4)
	{
	  int v1a = p->path[it->i], v1b = p->path[it->i + 1];
	  int v2a = v1b, v2b = p->path[it->i + 2];
	  if (it->s == 0)
	    {
	      // look for pair of M edges
	      cost_t c2 = distance(g, v2a, v2b);
	      printd("c2=%lld\n", c2);
	      if (c2 != g->bigM)
		{
		  it->i += 2;
		  it->j = 0;
		  continue;
		}

	      cost_t c1 = distance(g, v1a, v1b);
	      printd("c1=%lld\n", c1);
	      if (c1 != g->bigM)
		{
		  it->i += 1;
		  it->j = 0;
		  continue;
		}
	      it->s = 1;
	    }

	  if (it->j < p->length - 2 - (it->i == 0))
	    {
	      // skip edges next to the other two edge choices
	      if (it->j > it->i - 2 && it->j < (it->i + 3) % (p->length - 1))
		{
		  it->j = it->i + 3;
		  continue;
		}

	      int a = n->path[it->j], b = n->path[it->j + 1];
	      cost_t c = distance(g, v1a, v2b)
                       + distance(g, a, v1b)
                       + distance(g, b, v1b);
	      n->distance += c - 2 * g->bigM - distance(g, a, b);

	      if (it->j < it->i)
		{
		  path_reverse(n, 0, n->length);
		  path_move(n, n->length - it->i, n->length - (it->j + 1),
		      n->length - (it->i + 1));
		  path_reverse(n, 0, n->length);
		}
	      else
		{
		  path_move(n, it->i + 2, it->j, it->i + 1);
		}

	      it->j += 1;
	      return n;
	    }
	  else
	    {
	      it->i += 2;
	      it->j = 0;
	      it->s = 0;
	    }
	}
      else
	{
	  return NULL;
	}
    }
}

path * first_improv (graph * g, path * p, neighborhood_fn n_next, void* it)
{
  cost_t cost = cbtsp_o(g, p);
  cost_t candidate_cost = 0;
  path * candidate = NULL;
  for (;;)
    {
      candidate = n_next(g, p, it);
      if (candidate == NULL)
	{
	  return NULL;
	}
      else
	{
	  candidate_cost = cbtsp_o(g, candidate);
	  if (candidate_cost < cost)
	    {
	       return candidate;
	    }
	}
    }
}

path * best_improv (graph * g, path * p, neighborhood_fn n_next, void* it)
{
  cost_t candidate_cost = 0;
  path * candidate = NULL;
  cost_t best_candidate_cost = cbtsp_o(g, p);
  path * best_candidate = p;
  for (;;)
    {
      candidate = n_next(g, p, it);
      if (candidate == NULL)
	{
	  if (best_candidate == p)
	    {
	      return NULL;
	    }
	  else
	    {
	      return best_candidate;
	    }
	}
      else
	{
	  candidate_cost = cbtsp_o(g, candidate);
	  if (candidate_cost < best_candidate_cost)
	    {
	      if (best_candidate != p)
		{
		  free_path(best_candidate);
		}
	      best_candidate = candidate;
	      best_candidate_cost = candidate_cost;
	    }
	}
    }
}

path * rand_step (graph * g, path * p, neighborhood_fn n_next, void* it)
{
  int paths_i = 0;
  int paths_len = p->length;
  path ** paths = (path**)malloc(paths_len * sizeof(path*));
  path * candidate = NULL;
  cost_t cost = cbtsp_o(g, p);
  for (;;)
    {
      candidate = n_next (g, p, it);
      if (candidate == NULL)
      {
	if (paths_i == 0)
	  {
	    return NULL;
	  }
	int * indices = (int*)malloc(paths_i * sizeof(int));
	for (int i = 0; i < paths_i; ++i)
	  {
	    indices[i] = i;
	  }
	int tmp_i, shuffle_i;
	for (int i = 0; i < paths_i; ++i)
	  {
	    shuffle_i = floor(paths_i * rand_double());
	    tmp_i = indices[i];
	    indices[i] = indices[shuffle_i];
	    indices[shuffle_i] = tmp_i;
	  }

	path * n = NULL;
	int rand_index = -1;
	for (int i = 0; i < paths_i; ++i)
	  {
	    if (cbtsp_o(g, paths[indices[i]]) < cost)
	      {
		rand_index = indices[i];
		n = paths[rand_index];
		break;
	      }
	  }
	for (int i = 0; i < paths_i; ++i)
	  {
	    if (i != rand_index)
	      {
		free(paths[i]);
	      }
	  }
	free(paths);
	return n;
      }
      if (paths_i == paths_len)
	{
	  int new_paths_len = paths_len * 2;
	  path ** new_paths = (path**)malloc(new_paths_len * sizeof(path*));
	  memcpy(new_paths, paths, paths_len * sizeof(path*));
	  free(paths);
	  paths = new_paths;
	  paths_len = new_paths_len;
	}
      paths[paths_i++] = candidate;
    }
}

path * single_step (graph * g, path * p, neighborhood_fn n_next, void* it)
{
  return n_next(g, p, it);
}

path * local_search(graph * g, path * p, step_fn step, neighborhood_fn n_next, new_it_fn new_it, double runtime_seconds)
{
  clock_t beginning = clock();
  path * p_candidate = NULL;
  cost_t p_candidate_o = 0;
  path * best_candidate = p;
  cost_t best_candidate_cost = cbtsp_o(g, p);
  void * it = new_it();
  int max_fails = ceil(log(p->length));
  int fails = 0;
  while ((clock() - beginning) / CLOCKS_PER_SEC < runtime_seconds && fails < max_fails)
    {
      p_candidate = step(g, best_candidate, n_next, it);
      if (p_candidate == NULL)
	{
	  return best_candidate;
	}
      else
	{
	  p_candidate_o = cbtsp_o(g, p_candidate);
	  if (p_candidate_o < best_candidate_cost)
	    {
	      if (best_candidate != p)
		{
		  free_path(best_candidate);
		}
	      best_candidate = p_candidate;
	      best_candidate_cost = p_candidate_o;
	    }
	  else
	    {
	      fails += 1;
	    }
	  free(it);
	  it = new_it();
	}
    }
  free(it);
  return best_candidate;
}
