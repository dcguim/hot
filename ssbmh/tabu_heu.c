#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hamilt.h"
#include "tabu_heu.h"
#include "ls_heu.h"

void tabu_print(pair_edge * tabulist, int* tabuiter, int itabu)
{
  for (int i = 0; i<itabu;i++)
    {
      printf("[%d]\tniter: %d\n\t(%d , %d) (%d, %d)\n",
	     i, tabuiter[i], tabulist[i].e1->v1.id,
	     tabulist[i].e1->v2.id, tabulist[i].e2->v1.id,
	     tabulist[i].e2->v2.id);
    }
}
void remove_tabu(pair_edge * tabulist, int* tabuiter, int* len, int i)
{
  for (int j = i; j < *len; j++)
    {
      tabulist[j] = tabulist[j+1];
      tabuiter[j] = tabuiter[j+1];      
    }
  *len = *len -1;
}
void update_tabu(pair_edge * tabulist, int* tabuiter, int* len,
		 int tenure)
{
  for (int i = 0; i<*len; i++)
    {
      tabuiter[i]++;
      if (tabuiter[i] >= tenure)
	remove_tabu(tabulist, tabuiter, len, i);
    }  
}
path* tabu (graph* g, path* init)
{
  // length TL is half the number of nodes
  const int tabu_len = 1+(int)floor(g->m/2);
  pair_edge * tabulist = new_pair_edge(tabu_len);
  int * tabuiter = (int *) malloc (tabu_len*sizeof(int));
  int itabu= 0;
  // maximal number of iterations without improving solution
  int maxit = tabu_len;
  // tabu tenure, the time it takes for a instance in the tabulist
  //to be removed
  int tenure = (int)floor(g->m/2);
  int j = 0;

  pair_edge* ep = new_pair_edge(1);
  path* tpath = NULL;
  do{
    if (itabu == 0){
      tpath = ls_best_improv (g, init, NULL, 0,ep);}
    else {
      tpath = ls_best_improv (g, init, tabulist, itabu, ep);}
    if (init != tpath)
      {
	tabulist[itabu].e1->v1.id = ep->e1->v1.id;
	tabulist[itabu].e1->v2.id = ep->e1->v2.id;
	tabulist[itabu].e2->v1.id = ep->e2->v1.id;
	tabulist[itabu].e2->v2.id = ep->e2->v2.id;
	tabuiter[itabu] = 0;
	itabu = (itabu+1) % tabu_len;
	init = tpath;
	j = 0;
      }
    j++;
    update_tabu(tabulist, tabuiter, &itabu, tenure);
  } while (j < maxit);
  free(ep);
  free(tabuiter);
  free(tabulist);
  return init;
}
