#ifndef CONNECTIONS_IN
#define CONNECTIONS_IN

#include "tissue.h"
#include <stdlib.h>
#include <math.h>

/**
 * Gets the connections data set.  First dimension is from index, second dimension is to index
 */
static double rawConnections[NUM_CELLS][NUM_CELLS];

void cells_connectDirected(int fromIndex, int toIndex, double strength);
double cells_strengthOfConnection(int fromIndex, int toIndex);
int cells_countConnectedFrom(int fromIndex);
int * cells_indexesOfConnectedFrom(int fromIndex);
int * cells_indexesOfConnectedTo(int toIndex);

void cells_connectDirected(int fromIndex, int toIndex, double strength) {
    rawConnections[fromIndex][toIndex] = strength;
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

int cells_countConnectedTo(int toIndex) {
    int count = 0;
    int i;
    for(i = 0; i<NUM_CELLS; i++) {
        if(fabs(rawConnections[i][toIndex]) > 0.0) {
            count++;
        }
    }

    return count;
}

int * cells_indexesOfConnectedTo(int toIndex) {
    int count = cells_countConnectedTo(toIndex);
    
    if(count == 0) {
        return NULL;
    } else {
        int * ret = malloc(count * sizeof(int));
        int index = 0;
        int i;
        for(i = 0; i<NUM_CELLS; i++) {
            if(fabs(rawConnections[i][toIndex]) > 0.0) {
                ret[index] = i;
                index ++;
            }
        }

        return ret;
    }
}

int * cells_indexesOfConnectedFrom(int fromIndex) {

    int count = 0;
    int i;
    for(i = 0; i<NUM_CELLS; i++) {
        if(fabs(rawConnections[fromIndex][i]) > 0.0) {
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
            if(fabs(rawConnections[fromIndex][i]) > 0.0) {
                ret[index] = i;
                index ++;
            }
        }
    }

    return ret;
}

#endif