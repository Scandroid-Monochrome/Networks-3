#include <stdio.h>
#include "project3.h"
#include "utilities.h"

#define ON 1
#define OFF 0

extern int TraceLevel;
extern float clocktime;

char distance_vector0[20] = "";

struct distance_table dt0;
struct NeighborCosts   *neighbor0;

int find_min_cost(int start_num, int target_num, int current_min, struct distance_table* dt);

int find_min_cost_int(int start_num, int target_num, int starting_cost, int current_min, int path_tracker[MAX_NODES], int path_index_tracker, struct distance_table* dt);

void print_distance_table(struct distance_table dt, int table_num);

void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

void rtinit0() {
    neighbor0 = getNeighborCosts(0);
    init_node(0, *neighbor0, &dt0, distance_vector0);
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    print_distance_table(dt0, 0);

    // Find out which node our packet came from
    int rcv_index = rcvdpkt->sourceid;
    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, ", clocktime);
        printf("rtupdate0() called, by a pkt received from Sender id: %d.\n", rcv_index);
    }

    // Update our table with the new info
    update_table(&dt0, rcv_index, rcvdpkt->mincost);

    // Update the table based on our starting node values
    // Get our cost to the packet node
    int start_cost = dt0.costs[0][rcv_index];
    // Update our row using our new data and our starting cost
    update_row(&dt0, 0, rcv_index, start_cost);
}

void print_distance_table(struct distance_table dt, int table_num) {
    printf("\nDistance Table %d:\n", table_num);
    // Loop through each row:
    for (int row = 0; row < MAX_NODES; row++) {
        // Loop through each column:
        for (int col = 0; col < MAX_NODES; col++) {
            // Print our values:
            printf("%d, ", dt.costs[row][col]);
        }
        printf("\n");
    }
    printf("\n");
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


    // // Update the values for another node
    // // Be able to handle changing only 1, 2, or 3 values instead of all 4
    // for (int i = 0; i < 4; i++) {
    //     int current_cost = rcvdpkt->mincost[i];
    //     // If the cost is not 0(blank)
    //     if (current_cost != 0) {
    //         // Update table
    //         dt0.costs[starting_node][i] = current_cost;
    //     }
    // }

    // // Update our own values with the given information
    // int updated_values[MAX_NODES] = {0};
    // // Set up our variables
    // // int path_cost[MAX_NODES];
    // int path_tracker[MAX_NODES];

    // // Going from Node 0 to Nodes 1, 2, and 3
    // for (int node_num = 1; node_num < MAX_NODES; node_num++) {
    //     // Find the min path for each node and put it into an array
    //     int old_min_cost = dt0.costs[0][node_num];
    //     int updated_cost = find_min_cost(0, node_num, old_min_cost, &dt0);
    //     updated_values[node_num] = updated_cost;
    //     dt0.costs[0][node_num] =  updated_cost;
    // }

    // if (TraceLevel >= 1) {
    //     printf("At time t=%1.6f, ", clocktime);
    //     printf("node 0 current distance vector: %s\n", distance_vector0);
    // }

    // // printdt0(0, neighbor0, &dt0);

    // // Tell the other nodes what our new values are
    // for (int node_num = 1; node_num < MAX_NODES; node_num++) {
    //     struct RoutePacket update_pkt =  {0, node_num, updated_values};
    //     toLayer2(update_pkt);

    //     if (TraceLevel >= 1) {
    //         printf("At time t=%1.6f, ", clocktime);
    //         printf("node 0 sends packet to node %d with: %s\n", node_num, distance_vector0);
    //     }
    // }