#include <time.h>

#include "hamilt.h"
#include "grasp.h"

path * h_grasp (graph * g, double r, randomized_construction_f construct,
    local_search_f local_search, double runtime_seconds)
{
  time_t beginning = clock();
  path * p = local_search(g, construct(r));
  path * p_candidate;
  cost_t p_o = cbtsp_o(g, p);
  cost_t p_candidate_o;
  while ((clock() - beginning) / CLOCKS_PER_SEC < runtime_seconds)
    {
      p_candidate = local_search(g, construct(r));
      p_candidate_o = cbtsp_o(g, p_candidate);
      if (p_candidate_o < p_o)
	{
	  free_path(p);
	  p = p_candidate;
	  p_o = p_candidate_o;
	}
    }
  return p;
}
