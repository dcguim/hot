#ifdef PRINTD
#define printd(fmt, args...) printf(fmt, ##args)
#else
#define printd(fmt, args...)
#endif

struct edge;

typedef struct vertex
{
  int id;
  int degree;
  struct edge * edges;
} vertex;

typedef struct edge
{
  vertex v1, v2;
  int c;
} edge;

typedef struct graph
{
  int n, m, bigM;
  edge * edges;
  vertex * vertices;
} graph;

typedef struct path
{
  int * path;
  int length;
} path;


path * new_path (int length);

void path_print(path * p);

int cmp_int (const void * a, const void * b);

int cmp_cost (const void * a, const void * b);

double rand_double ();
