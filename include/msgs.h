#ifndef KSIGNALS_IN
#define KSIGNALS_IN

#include "../include/connections.h"
#include "../include/math.h"
#include "../include/logger.h"

double * cells_rawOutput(int inputCellIndex, double inputStrength);

/**
 * Stimulate the tissue at the given target cells with the given strengths.  Count is passed as a 
 * convenience to cut down on needing to work out array lengths.
 */
void cells_stimulate(int * targets, double * strengths, int count);

double * cells_rawOutput(int inputCellIndex, double inputStrength) {

    int size = cells_countConnectedFrom(inputCellIndex);
    int * endpointIndexes = cells_indexesOfConnectedFrom(inputCellIndex);
    double connectionStrengths[size];
    
    int i;
    for(i=0; i<size; i++) {
        connectionStrengths[i] = cells_strengthOfConnection(inputCellIndex, endpointIndexes[i]);
    }

    return kmath_linalg_scalar_times_vec(connectionStrengths, size, inputStrength);

}

void cells_stimulate(int * targets, double * strengths, int count) {
    
}

#endif