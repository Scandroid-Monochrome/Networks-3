// #include "project3.h"
struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

void init_node(int current_node, struct NeighborCosts neighborCosts, struct distance_table *dt);