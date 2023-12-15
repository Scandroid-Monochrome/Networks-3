#include <stdio.h>
#include "project3.h"
#include "utilities.h"

#define ON 1
#define OFF 0

extern int TraceLevel;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt0;
struct NeighborCosts   *neighbor0;

int find_min_cost(int start_num, int target_num, int current_min, struct distance_table* dt);

int find_min_cost_int(int start_num, int target_num, int starting_cost, int current_min, int path_tracker[MAX_NODES], int path_index_tracker, struct distance_table* dt);

void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

void rtinit0() {
    // Find our neighbor nodes and the costs to those nodes
    neighbor0 = getNeighborCosts(0);

    // Variable for our costs
    int our_costs[MAX_NODES];

    // Initialize the distance table and other structures
    // init_distance_table(&dt0, neighbor0, 0);
    for (int nodenum = 0; nodenum < 4; nodenum++) {
        int node_cost = neighbor0->NodeCosts[nodenum];
        dt0.costs[0][nodenum] = node_cost;
        our_costs[nodenum] = node_cost;
        // printf("%d,", our_costs[nodenum]);
    }
    // printf("\n");

    // Send minimum cost paths to neighbor nodes
    for (int nodenum = 1; nodenum < 4; nodenum++) {
        struct RoutePacket our_packet = {0, nodenum, our_costs};
        toLayer2(our_packet);
    }

}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    printdt0(0, neighbor0, &dt0);
    // Find out which node our packet came from
    int starting_node = rcvdpkt->sourceid;

    // Update the values for another node
    // Be able to handle changing only 1, 2, or 3 values instead of all 4
    for (int i = 0; i < 4; i++) {
        int current_cost = rcvdpkt->mincost[i];
        // If the cost is not 0(blank)
        if (current_cost != 0) {
            // Update table
            dt0.costs[starting_node][i] = current_cost;
        }
    }

    // Update our own values with the given information
    int updated_values[MAX_NODES] = {0};
    // Set up our variables
    // int path_cost[MAX_NODES];
    int path_tracker[MAX_NODES];

    // Going from Node 0 to Nodes 1, 2, and 3
    for (int node_num = 1; node_num < MAX_NODES; node_num++) {
        // Find the min path for each node and put it into an array
        int old_min_cost = dt0.costs[0][node_num];
        int updated_cost = find_min_cost(0, node_num, old_min_cost, &dt0);
        updated_values[node_num] = updated_cost;
        dt0.costs[0][node_num] =  updated_cost;
    }

    printdt0(0, neighbor0, &dt0);

    // Tell the other nodes what our new values are
    for (int node_num = 1; node_num < MAX_NODES; node_num++) {
        struct RoutePacket update_pkt =  {0, node_num, updated_values};
        toLayer2(update_pkt);
    }
}

int find_min_cost(int start_num, int target_num, int current_min, struct distance_table* dt) {
    int path_cost[MAX_NODES];
    int path_tracker[MAX_NODES];

    // We start at the first node, guard to keep min cost from becoming zero
    path_tracker[0] = start_num;

    return find_min_cost_int(start_num, target_num, 0, current_min, path_tracker, 1, dt);
}

int find_min_cost_int(int start_num, int target_num, int accum_cost, int current_min, int path_tracker[MAX_NODES], int path_index_tracker, struct distance_table* dt) {
    // Create an array for our path cost combos
    int combo_costs[MAX_NODES] = {999};

    // Scan our connected nodes for their data
    for (int scan_node = 0; scan_node < MAX_NODES; scan_node++) {
        // If the scan node is the target:
        if (scan_node == target_num) {
            accum_cost += dt->costs[start_num][scan_node];
            // Challenge our accumulated cost to the current minimum
            if (accum_cost < current_min) {
                current_min = accum_cost;
            }
            // Return with the result
            return current_min;
        // If we already explored the node:
        } else if (already_explored(path_tracker, scan_node)) {
            continue;
        // If node is an unexplored, non-target node:
        } else {
            // Recurse and reiterate
            accum_cost += dt->costs[start_num][scan_node];
            path_tracker[path_index_tracker] = scan_node;
            combo_costs[scan_node] = find_min_cost_int(scan_node, target_num, accum_cost, current_min, path_tracker, path_index_tracker + 1, dt);
            // branches ++;
        }
        
        // For each branch
        for (int i = 0; i < MAX_NODES; i++) {
            // If the branch minimum cost is less than our current minimum cost:
            if (combo_costs[i] < current_min) {
                // Replace our current minimum cost with it
                current_min = combo_costs[i];
            }
        }
        return current_min;
    }

    return accum_cost;
}

// Make sure we didn't already explore the node
int already_explored(int explored_nodes[MAX_NODES], int node_num) {
    int already_explored = 0;

    // Check to see if we already explored the node
    for (int current_node = 0; current_node < MAX_NODES; current_node++) {
        // If we did:
        if (current_node == node_num) {
            // Set already_explored to true
            already_explored = 1;
            break;
        }
    }

    return already_explored;
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt0

