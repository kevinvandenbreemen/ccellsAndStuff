#ifndef KSIGNALS_IN
#define KSIGNALS_IN

#include "../lib/connections.h"
#include "../lib/math.h"
#include "../lib/logger.h"

double * cells_rawOutput(int inputCellIndex, double inputStrength) {

    int size = cells_countConnectedFrom(inputCellIndex);
    int * endpointIndexes = cells_indexesOfConnectedFrom(inputCellIndex);
    double connectionStrengths[size];
    
    int i;
    for(i=0; i<size; i++) {
        connectionStrengths[i] = cells_strengthOfConnection(inputCellIndex, endpointIndexes[i]);
    }

    printf("CONECTION STRENGTHS0:  %f\n", connectionStrengths[0]);
    printf("CONECTION STRENGTHS1:  %f\n", connectionStrengths[1]);

    double * ret = kmath_linalg_scalar_times_vec(connectionStrengths, size, inputStrength);

    printf("RESULT0:  %f\n", ret[0]);
    printf("RESULT1:  %f\n", ret[1]);

    return ret;

}

#endif