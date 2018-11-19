#ifndef GRASP_H
#define GRASP_H

#include "hamilt.h"

typedef path * (*randomized_construction_f)(double r);

typedef path * (*local_search_f)(graph * g, path * p);

path * h_grasp (graph * g, double r, randomized_construction_f construct,
    local_search_f local_search, double runtime_seconds);

#endif
