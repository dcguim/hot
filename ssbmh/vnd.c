#include <stdlib.h>
#include <stdio.h>
#include "hamilt.h"
#include "ls_heu.h"
#include "vnd.h"

typedef path * (*neighbor)(path * p);

graph * g;

path * n1(path * p)
{
  path * n = ls_best_improv(g, p);
  if (n == p)
    {
      return NULL;
    }
  return n;
}

path * n2(path * p)
{
  n_3opt_it * it = n_3opt_new_it();
  path * n = first_improv(g, p, n_3opt_next, it);
  free(it);
  return n;
}

path * n3(path * p)
{
  n_25opt_it * it = n_25opt_new_it();
  path * n = first_improv(g, p, n_25opt_next, it);
  free(it);
  return n;
}

#define N_LENGTH 3

path * h_vnd(graph * g, path * p)
{
  neighbor neighborhoods[] = {n3, n1, n2};
  int neighborhood_i = 0;
  path * best_candidate = copy_path(p);
  cost_t best_candidate_o = cbtsp_o(g, best_candidate);
  path * p_candidate = NULL;
  cost_t p_candidate_o = 0;
  while (neighborhood_i < N_LENGTH)
    {
      p_candidate = neighborhoods[neighborhood_i](best_candidate);
      if (p_candidate != NULL)
        {
          p_candidate_o = cbtsp_o(g, p_candidate);
          if (p_candidate_o < best_candidate_o)
          {
            printd("N%d: %lld\n", neighborhood_i, p_candidate_o);
            free_path(best_candidate);
            best_candidate = p_candidate;
            best_candidate_o = p_candidate_o;
            neighborhood_i = 0;
            continue;
          }
        }
      neighborhood_i += 1;
      printd("N%d -> N%d\n", neighborhood_i - 1, neighborhood_i);
    }
  return best_candidate;
}
