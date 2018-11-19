#ifndef HAMILT_H
#define HAMILT_H

#ifdef PRINTD
#define printd(fmt, args...) printf(fmt, ##args)
#else
#define printd(fmt, args...)
#endif

struct edge;

typedef long long int cost_t;

typedef struct vertex
{
  int id;
  int degree;
  struct edge * edges;
} vertex;

typedef struct edge
{
  vertex v1, v2;
  cost_t c;
} edge;

typedef struct graph
{
  int n, m;
  cost_t bigM;
  edge * edges;
  vertex * vertices;
} graph;

typedef struct path
{
  int * path;
  int length;
} path;

path * new_path (int length);

void free_path (path * p);

void path_print(path * p);

void edges_print(graph * g);

int cmp_int (const void * a, const void * b);

int cmp_cost (const void * a, const void * b);

cost_t cbtsp_o(graph * g, path * p);

double rand_double();

edge * find_edge (graph * g, int a, int b);

path* invert (path* p, int beg, int end);

int feasible(path * p);

path * copy_path (path* p);

path * assign_path (path* a, path* p);

#endif
