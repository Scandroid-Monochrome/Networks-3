#include <stdio.h>
#include "project3.h"
#include "utilities.h"

extern int TraceLevel;
extern float clocktime;

void init_node(int current_node, struct NeighborCosts neighborCosts, struct distance_table *dt) {
    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, rtinit%d called.\n", current_node, clocktime);
    }
    // Find our neighbor nodes and the costs to those nodes
    // neighborCosts[current_node] = getNeighborCosts(current_node);

    // Variable for our costs
    int our_costs[MAX_NODES];

    char distance_vector[20] = "";

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
