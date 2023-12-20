#include <stdio.h>
#include "project3.h"
#include "utilities.h"

extern int TraceLevel;
extern float clocktime;

void init_node(int current_node, struct NeighborCosts neighborCosts, struct distance_table *dt, char distance_vector[20]) {
    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, rtinit%d called.\n", current_node, clocktime);
    }
    // Find our neighbor nodes and the costs to those nodes
    // neighborCosts[current_node] = getNeighborCosts(current_node);

    // Variable for our costs
    int our_costs[MAX_NODES];

    // char distance_vector[20] = "";

    // Initialize the distance table and other structures
    // init_distance_table(&dt0, neighbor0, 0);
    for (int nodenum = 0; nodenum < 4; nodenum++) {
        int node_cost = neighborCosts.NodeCosts[nodenum];
        dt->costs[current_node][nodenum] = node_cost;
        our_costs[nodenum] = node_cost;
        sprintf(distance_vector, "%s%d ", distance_vector, node_cost);
        // printf("%d,", our_costs[nodenum]);
    }
    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, ", clocktime);
        printf("node %d initial distance vector: %s\n", current_node, distance_vector);
    }
    // printf("\n");

    // Send minimum cost paths to neighbor nodes
    for (int nodenum = 0; nodenum < 4; nodenum++) {
        if (nodenum != current_node) {
            struct RoutePacket our_packet;
            our_packet.sourceid = current_node;
            our_packet.destid = nodenum;
            memcpy(our_packet.mincost, our_costs, MAX_NODES*sizeof(int));
            toLayer2(our_packet);

            if (TraceLevel >= 1) {
                printf("At time t=%1.6f, ", clocktime);
                printf("node %d sends packet to node %d with: %s\n", current_node, nodenum, distance_vector);
            }
        }
    }
}



void update_table(struct distance_table* dt, int index, int new_info[MAX_NODES]) {
    // Loop through our row
    for (int i = 0; i < MAX_NODES; i++) {
        // List our update variable
        int new_value = new_info[i];
        // Update our table with the new info
        dt->costs[index][i] = new_value;
    }
}

void update_row(struct distance_table* dt, int start_row, int end_row, int start_cost) {
    // Loop through our distance table
    for (int i = 0; i < MAX_NODES; i++) {
        
        // Log our current cost for this node
        int current_cost = dt->costs[start_row][i];

        // If the cell isn't our starting cell
        if (i != end_row) {

            int scnd_hop_cost = dt->costs[end_row][i];
            // Add our start cost to the cost from this node to the next
            int new_cost = start_cost + scnd_hop_cost;

            // Compare our new cost to our current cost
            // If our new cost is less:
            if (new_cost < current_cost) {
                // Set our current cost to new cost
                dt->costs[start_row][i] = new_cost;
            }
        }
    }
}
