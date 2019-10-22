#include "../include/connections.h"
#include "../include/tissue.h"
#include "../include/msgs.h"

static void test_signalIncomingConnections(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths) {
    printf("PERF:  signalling incoming connections to %d -- %d connections\n", cellIndex, size);
}

static void test_signalOutgoingConnections(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths) {
    printf("PERF:  signalling outgoing connections from %d -- %d connections\n", cellIndex, size);
}

int main() {

    printf("Tissue System Mem Leak / Perf Tester\n");

    tissue_initializeDefaultTissue();
    cellTypes_setCellLogicForOutgoingConnections(CELL_TYPE_BASIC, test_signalOutgoingConnections);
    cellTypes_setCellLogicForIncomingConnections(CELL_TYPE_BASIC, test_signalIncomingConnections);

    cells_connectDirected(0, 10, 0.24);
    cells_connectDirected(1, 10, 2.2);
    cells_connectDirected(2, 10, -0.4);
    cells_connectDirected(3, 10, 4.9);
    cells_connectDirected(2, 3, 4.9);

    int targets[] = {0,1,2,3};
    double strengths[] = {1.0, 1.0, 10.3, 1.2};
    int count = 4;
    
    cells_stimulate(targets, strengths, count);

    TissueState * state = tissue_getState();

    free(state->outputIndices);
    free(state->outputStrengths);
    free(state);

    return 0;
}