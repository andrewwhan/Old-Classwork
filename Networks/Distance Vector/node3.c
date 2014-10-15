#include <stdio.h>
#include <string.h>
#include "project3.h"

#define MY_NUMBER 3

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt3;
struct NeighborCosts   *neighbor3;

//Function prototype for access within rtinit1() and rtupdate1()
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor, 
        struct distance_table *dtptr );

/* students to write the following two routines, and maybe some others */

void rtinit3() {
    if(TraceLevel > 0){
        printf("At time t=%f, rtinit3() called. \n", clocktime);
    }
    neighbor3 = getNeighborCosts(MY_NUMBER);
    int i, j;
    //Populate distance table with neighbor costs and infinity for everyone else
    for(i=0; i<MAX_NODES; i++){
        for(j=0; j<MAX_NODES; j++){
            if(j != MY_NUMBER){
                dt3.costs[i][j] = INFINITY;
            }
            else{
                dt3.costs[i][j] = neighbor3->NodeCosts[i];
            }
        }
    }
    //Send packets to all neighbors with this node's neighbor costs
    for(i=0; i<MAX_NODES; i++){
        if(neighbor3->NodeCosts[i] != INFINITY && i != MY_NUMBER){
            int costs[MAX_NODES];
            for(j=0; j<MAX_NODES; j++){
                costs[j] = dt3.costs[j][MY_NUMBER];
            }
            struct RoutePacket toSend = {MY_NUMBER, i};
            memcpy(toSend.mincost, costs, sizeof(int)*MAX_NODES);
            if(TraceLevel > 0){
                printf("At time t=%f, node %d sends packet to node %d with:", clocktime,
                    MY_NUMBER, i);
                    for(j=0; j<MAX_NODES; j++){
                        printf(" %d", dt3.costs[j][MY_NUMBER]);
                    }
                    printf("\n");
            }
            toLayer2(toSend);
        }
    }
    if(TraceLevel > 0){
        printdt3(MY_NUMBER, neighbor3, &dt3);
    }
}


void rtupdate3( struct RoutePacket *rcvdpkt ) {
    int i, j;
    int updated = 0;
    //Update distance table 
    for(i=0; i<MAX_NODES; i++){
        //If neighbor has a new minimum cost to another node, update it in its distance table
        if(dt3.costs[i][rcvdpkt->sourceid] > rcvdpkt->mincost[i]){
            dt3.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i];
            //If this would creates a new shorter path to another node from this node, update it in the distance table
            if(dt3.costs[i][MY_NUMBER] > dt3.costs[rcvdpkt->sourceid][MY_NUMBER] + rcvdpkt->mincost[i]){
                dt3.costs[i][MY_NUMBER] = dt3.costs[rcvdpkt->sourceid][MY_NUMBER] + rcvdpkt->mincost[i];
                updated = 1; //Flag that this node has a new path to another node
            }
        }
        
    }
    if(TraceLevel > 0){
        printf("At time t=%f, node %d receives a packet from node %d. The updated distance table follows: \n",
            clocktime, MY_NUMBER, rcvdpkt->sourceid);
        printdt3(MY_NUMBER, neighbor3, &dt3);
    }
    //If this node has a shorter path to another node, broadcast this new information
    if(updated){
        for(i=0; i<MAX_NODES; i++){
            if(neighbor3->NodeCosts[i] != INFINITY && i != MY_NUMBER){
                int costs[MAX_NODES];
            for(j=0; j<MAX_NODES; j++){
                costs[j] = dt3.costs[j][MY_NUMBER];
            }
            struct RoutePacket toSend = {MY_NUMBER, i};
            memcpy(toSend.mincost, costs, sizeof(int)*MAX_NODES);
                if(TraceLevel > 0){
                    printf("At time t=%f, node %d sends packet to node %d with:",
                        clocktime, MY_NUMBER, i);
                    for(j=0; j<MAX_NODES; j++){
                        printf(" %d", dt3.costs[j][MY_NUMBER]);
                    }
                    printf(" to reflect updated distance table. \n");
                }
                toLayer2(toSend);
            }
        }
    }
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
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt3

