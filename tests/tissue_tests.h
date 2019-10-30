#ifndef IN_TISSUE_TESTS
#define IN_TISSUE_TESTS

#include <check.h>
#include "../include/tissue.h"
#include "../include/connections.h"
#include "../include/cellTypes.h"

START_TEST(set_cell_type) {
    tissue_setCellType(10, CELL_TYPE_INHIB);
    fail_unless(tissue_getCellType(10) == CELL_TYPE_INHIB, "System should have updated cell type");
}
END_TEST

START_TEST(cell_type_out_of_bounds) {
    tissue_setCellType(NUM_CELLS, CELL_TYPE_BASIC);
    fail_unless(tissue_getCellType(NUM_CELLS) == -1, "System should have gracefully handled out of bounds! - got %d", tissue_getCellType(NUM_CELLS));
}
END_TEST

START_TEST(initialize_cell_type_behaviours) {
    cellTypes_AllocateCellTypeBehaviours();
}
END_TEST

START_TEST(initialize_default_cell_type_behaviours) {
    cellTypes_InitializeDefaultCellTypeBehaviours();
}
END_TEST

START_TEST(get_default_cell_type_behaviours) {
    cellTypes_InitializeDefaultCellTypeBehaviours();
    CellTypeBehaviour *basicCellTypeBehaviour = cellTypes_behaviourFor(CELL_TYPE_BASIC);

    fail_if(basicCellTypeBehaviour == NULL, "System should have initialized 'basic' cell type behaviour");
    double outputStrength = basicCellTypeBehaviour->getOutputStrength(0, 0.5, 0.5);
    fail_unless(outputStrength == 0.5, "Output Strength should be base input strength * output connection strength");

    basicCellTypeBehaviour = cellTypes_behaviourFor(CELL_TYPE_INHIB);
    fail_if(basicCellTypeBehaviour == NULL, "System should have initialized 'inhibitory' cell type behaviour");

}
END_TEST

//  Cell updates/learning callbacks!
int callCount;
int expectedIndex;
int signalIncoming_size;
int signalIncoming_cellIndex;
int * signalIncoming_incomingIndexes;
double * signalIncoming_incomingStrengths;
static void test_signalIncomingConnections(int cellType, int size, int cellIndex, int * incomingIndexes, double * incomingStrengths) {
    if(cellIndex != expectedIndex){
        return;
    }

    callCount ++;

    signalIncoming_size = size;
    signalIncoming_cellIndex = cellIndex;
    
    signalIncoming_incomingIndexes = malloc(sizeof(int) * signalIncoming_size);
    signalIncoming_incomingStrengths = malloc(sizeof(double) * signalIncoming_size);
    
    int i;
    for(i=0; i<signalIncoming_size; i++) {
        signalIncoming_incomingIndexes [i] = incomingIndexes[i];
        signalIncoming_incomingStrengths[i] = incomingStrengths[i];
    }

    printf("Incoming Connections - size=%d, index=%d\n", size, cellIndex);

}

int signalOutgoing_size;
int signalOutgoing_cellIndex;
int * signalOutgoing_outgoingIndexes;
double * signalOugtoing_outgoingStrengths;
static void test_signalOutgoingConnections(int cellType, int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths) {
    if(cellIndex != expectedIndex){
        printf("NOT %d but is %d, so can't do outgoing\n", expectedIndex, cellIndex);
        return;
    }

    callCount ++;

    signalOutgoing_size = size;
    signalOutgoing_cellIndex = cellIndex;
    
    signalOutgoing_outgoingIndexes = malloc(sizeof(int) * signalOutgoing_size);
    signalOugtoing_outgoingStrengths = malloc(sizeof(double) * signalOutgoing_size);
    int i;
    for(i=0; i<signalOutgoing_size; i++) {
        signalOutgoing_outgoingIndexes[i] = outgoingIndexes[i];
        signalOugtoing_outgoingStrengths[i] = outgoingStrengths[i];
    }
}

START_TEST(process_incoming_cell_connections_during_network_stim) {
    tissue_initializeDefaultTissue();
    cellTypes_InitializeDefaultCellTypeBehaviours();
    cellTypes_setCellLogicForIncomingConnections(CELL_TYPE_BASIC, *test_signalIncomingConnections);
    expectedIndex = 1;

    cells_connectDirected(0, 1, 0.1);
    cells_connectDirected(2, 1, 0.2);

    int targets[] = {0, 2};
    double strengths[] = {1.5, 1.5};
    cells_stimulate(targets, strengths, 2);

    fail_unless(signalIncoming_size == 2, "System should have alerted cells to conduct operations");
    fail_unless(signalIncoming_cellIndex == 1, "System should have alerted cell at index 1 that it needs to update its logic");
    fail_unless(signalIncoming_incomingIndexes[0] == 0);
    fail_unless(signalIncoming_incomingIndexes[1] == 2, "Expected incoming index at 1 to be 2 but was %d\n", signalIncoming_incomingIndexes[1]);
    fail_unless(signalIncoming_incomingStrengths[0] == 0.1);
    fail_unless(signalIncoming_incomingStrengths[1] == 0.2);
    fail_if(callCount != 1, "System should only process incoming connections per cell once, but did so %d times", callCount);
}
END_TEST

START_TEST(process_outgoing_cell_connections_during_network_stim) {

    tissue_initializeDefaultTissue();
    cellTypes_InitializeDefaultCellTypeBehaviours();
    cellTypes_setCellLogicForOutgoingConnections(CELL_TYPE_BASIC, *test_signalOutgoingConnections);
    expectedIndex = 2;

    cells_connectDirected(0, 1, 0.1);
    cells_connectDirected(2, 1, 0.2);

    int targets[] = {0, 2};
    double strengths[] = {1.5, 1.5};
    cells_stimulate(targets, strengths, 2);

    fail_unless(signalOutgoing_size == 1, "System should have alerted cells to conduct operations");
    fail_unless(signalOutgoing_cellIndex == 2, "System should have alerted cell at index 1 that it needs to update its logic");
    fail_unless(signalOutgoing_outgoingIndexes[0] == 1, "Outgoing indexes expected 1 but was %d\n", signalOutgoing_outgoingIndexes[0]);
    fail_unless(signalOugtoing_outgoingStrengths[0] == 0.2);
    fail_if(callCount != 1, "System should only process outgoing connections per cell once, but did so %d times", callCount);
}
END_TEST

START_TEST(process_incoming_cell_connections_during_feedforward) {

    //  Arrange
    expectedIndex = 3;
    
    tissue_initializeDefaultTissue();
    cellTypes_InitializeDefaultCellTypeBehaviours();
    cellTypes_setCellLogicForIncomingConnections(CELL_TYPE_BASIC, *test_signalIncomingConnections);

    cells_connectDirected(0, 2, 1.0);
    cells_connectDirected(0, 3, 1.5);
    cells_connectDirected(1, 2, 1.0);
    cells_connectDirected(1, 3, 0.3);
    cells_connectDirected(1, 7, 0.5);

    //  Act
    int targets[] = {0, 1};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_matrix_feedforward_stim(targets, strengths, count);

    //  Assert
    fail_unless(signalIncoming_size == 2, "System should have alerted cells to conduct operations - size=%d", signalIncoming_size);
    fail_unless(signalIncoming_cellIndex == 3, "System should have alerted cell at index 3 that it needs to update its logic");
    fail_unless(signalIncoming_incomingIndexes[0] == 0);
    fail_unless(signalIncoming_incomingIndexes[1] == 1, "Expected incoming index at 1 to be 2 but was %d\n", signalIncoming_incomingIndexes[1]);
    fail_unless(signalIncoming_incomingStrengths[0] == 1.5);
    fail_unless(signalIncoming_incomingStrengths[1] == 0.3);
    fail_if(callCount != 1, "System should only process incoming connections per cell once, but did so %d times", callCount);


} END_TEST

TissueState *capturedState;
void captureTissueStateUpdate(TissueState * state) {
    capturedState = state;
}

START_TEST(send_state_to_listener_on_complete) {
    //  Arrange
    tissue_setOnStateUpdate(*captureTissueStateUpdate);

    tissue_initializeDefaultTissue();

    cells_connectDirected(10, 5, 1.0);
    cells_connectDirected(11, 6, 1.5);
    cells_connectDirected(5, 2, 1.0);
    cells_connectDirected(6, 3, 0.3);
    cells_connectDirected(5, 7, 0.5);

    //  Act
    int targets[] = {10, 11};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_matrix_feedforward_stim(targets, strengths, count);

    //  Assert
    TissueState * state = capturedState;
    fail_if(state == NULL, "System should have sent back state data");
    fail_unless(state->outputCount == 3, "3 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;

    fail_unless(destIndexes[0] == 2);
    fail_unless(destIndexes[1] == 3);
    fail_unless(destIndexes[2] == 7);

    int i;
    for(int i=0; i<3; i++) {
        printf("WTF strength of %d = %f\n", destIndexes[i], destStrengths[i]);
    }

    fail_unless(destStrengths[0] == 1.0);
    fail_unless(destStrengths[2] == 0.5);

}
END_TEST

START_TEST (tissue_reset) {
    tissue_initializeDefaultTissue();

    cells_connectDirected(0, 2, 1.0);
    cellTypes_setCellLogicForIncomingConnections(CELL_TYPE_BASIC, *test_signalIncomingConnections);

    tissue_setCellType(1, 2);

    tissue_resetAll();

    fail_unless(tissue_getCellType(1) == 1, "System should reset cell types as part of init tissue");
    fail_unless(cells_countConnectedFrom(0) == 0, "System should have reset connections between cells in tissue");
    fail_unless(cellTypes_existsLogicForIncomingConnectionsForCellType(CELL_TYPE_BASIC) == 0, "System should reset handler logic");
    fail_unless(tissue_getState()->outputCount == 0, "System should have reset tissue state");

}
END_TEST

START_TEST (get_number_of_cells) {

    fail_unless(tissue_getNumCells() == NUM_CELLS);

}
END_TEST

void tissue_tests_addToSuite(Suite *suite) {
    TCase *tissueTests = tcase_create("Tissue/Cell Type Tests");

    tcase_add_test(tissueTests, set_cell_type);
    tcase_add_test(tissueTests, cell_type_out_of_bounds);
    tcase_add_test(tissueTests, initialize_cell_type_behaviours);
    tcase_add_test(tissueTests, initialize_default_cell_type_behaviours);
    tcase_add_test(tissueTests, get_default_cell_type_behaviours);
    tcase_add_test(tissueTests, process_incoming_cell_connections_during_network_stim);
    tcase_add_test(tissueTests, process_outgoing_cell_connections_during_network_stim);
    tcase_add_test(tissueTests, process_incoming_cell_connections_during_feedforward);
    tcase_add_test(tissueTests, tissue_reset);
    tcase_add_test(tissueTests, send_state_to_listener_on_complete);
    tcase_add_test(tissueTests, get_number_of_cells);

    suite_add_tcase(suite, tissueTests);
}

#endif