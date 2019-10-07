#ifndef CONNECTIONS_IN
#define CONNECTIONS_IN

#include "tissue.h"
#include <stdlib.h>

/**
 * Gets the connections data set.  First dimension is from index, second dimension is to index
 */
static double rawConnections[NUM_CELLS][NUM_CELLS];

void cells_connectDirected(int fromIndex, int toIndex, double strength);
void cells_connectBidirectional(int fromIndex, int toIndex, double strength);
double cells_strengthOfConnection(int fromIndex, int toIndex);
int cells_countConnectedFrom(int fromIndex);
int * cells_indexesOfConnectedFrom(int fromIndex);

void cells_connectDirected(int fromIndex, int toIndex, double strength) {
    rawConnections[fromIndex][toIndex] = strength;
}

void cells_connectBidirectional(int fromIndex, int toIndex, double strength) {
    rawConnections[fromIndex][toIndex] = strength;
    rawConnections[toIndex][fromIndex] = strength;
}

double cells_strengthOfConnection(int fromIndex, int toIndex) {
    return rawConnections[fromIndex][toIndex];
}

int cells_countConnectedFrom(int fromIndex) {
    int count = 0;
    int i;
    for(i = 0; i<NUM_CELLS; i++) {
        if(rawConnections[fromIndex][i] > 0.0) {
            count++;
        }
    }

    return count;
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

        ret = malloc(sizeof(int) * count);

        int index = 0;
        for(i = 0; i<NUM_CELLS; i++) {
            if(rawConnections[fromIndex][i] > 0.0) {
                ret[index] = i;
                index ++;
            }
        }
    }

    return ret;
}

#endif