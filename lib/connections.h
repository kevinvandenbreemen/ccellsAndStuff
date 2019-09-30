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

void cells_connectBidirectional(int fromIndex, int toIndex, double strength) {
    rawConnections[fromIndex][toIndex] = strength;
    rawConnections[toIndex][fromIndex] = strength;
}

int * cells_indexesOfConnectedFrom(int fromIndex) {

    int count = 0;
    int i;
    for(i = 0; i<NUM_CELLS; i++) {
        if(rawConnections[fromIndex][i] > 0.0) {
            count++;
        }
    }

    int *ret;
    if(count == 0) {
        ret = NULL;
    } else {

        int index = 0;
        int tmp[count];
        for(i = 0; i<NUM_CELLS; i++) {
            if(rawConnections[fromIndex][i] > 0.0) {
                tmp[index] = i;
                index ++;
            }
        }

        ret = tmp;
    }

    return ret;
}

#endif