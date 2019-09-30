#ifndef CONNECTIONS_IN
#define CONNECTIONS_IN

#include "tissue.h"
#include <stdlib.h>

/**
 * Gets the connections data set.  First dimension is from index, second dimension is to index
 */
static double rawConnections[NUM_CELLS][NUM_CELLS];

void cells_connectDirected(int fromIndex, int toIndex, double strength) {
    rawConnections[fromIndex][toIndex] = strength;
}



#endif