// #include "project3.h"
struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

struct cost_tuple {
  int node_num;
  int cost;
};

void init_node(int current_node, struct NeighborCosts neighborCosts, struct distance_table *dt, char distance_vector[20]);

void update_table(struct distance_table* dt, int index, int new_info[MAX_NODES]);

void update_row(struct distance_table* dt, int start_row, int end_row, int start_cost);