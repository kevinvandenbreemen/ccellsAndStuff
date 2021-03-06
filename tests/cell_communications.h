#include <check.h>
#include "../include/msgs.h"
#include "../include/tissue.h"

START_TEST(message_raw_data_to_dest_cells) {

    tissue_initializeDefaultTissue();

    cells_connectDirected(10, 11, 0.5);
    cells_connectDirected(10, 12, 0.1);

    double * output = cells_rawOutput(10, 2.0);
    
    fail_unless(output[0] == 1.0, "Output for for (10->11)[strength=0.5] for input 2.0 should be 1.0 but was %f", output[0]);
    fail_unless(output[1] == 0.2, "Output for for (10->12)[strength=0.1] for input 2.0 should be 0.2 but was %f", output[1]);

}
END_TEST

START_TEST(message_propagate_data_to_cells) {

    tissue_initializeDefaultTissue();

    cells_connectDirected(10, 11, 0.5);
    cells_connectDirected(10, 12, 0.1);
    cells_connectDirected(11, 13, 1.0);

    int targets[] = {10, 11};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_stimulate(targets, strengths, count);

    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 3, "3 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;

    fail_unless(destIndexes[0] == 13, "First index should be 13 but was %d", destIndexes[0]);
    fail_unless(destStrengths[0] == 0.5, "First strength should be 0.5, but was %f", destStrengths[0]);

    fail_unless(destIndexes[1] == 12, "First index should be 13 but was %d", destIndexes[1]);
    fail_unless(destStrengths[1] == 0.1, "First strength should be 0.5, but was %f", destStrengths[1]);

    fail_unless(destIndexes[2] == 13, "First index should be 13 but was %d", destIndexes[2]);
    fail_unless(destStrengths[2] == 1, "First strength should be 0.5, but was %f", destStrengths[2]);

}
END_TEST

START_TEST(feedfoward_stimulation) {

    //  Arrange
    tissue_initializeDefaultTissue();

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
    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 3, "3 outputs expected, got %d", state->outputCount);

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;

    fail_unless(destIndexes[0] == 2);
    fail_unless(destIndexes[1] == 3);
    fail_unless(destIndexes[2] == 7);

    fail_unless(destStrengths[0] == 2.0);
    fail_unless(destStrengths[1] == 1.8);
    fail_unless(destStrengths[2] == 0.5);
}
END_TEST

START_TEST(feedfoward_stimulation_multilayer) {

    //  Arrange
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
    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 2, "2 outputs expected, got %d", state->outputCount);

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;

    fail_unless(destIndexes[0] == 5);
    fail_unless(destIndexes[1] == 6);

    fail_unless(destStrengths[0] == 1.0);
    fail_unless(destStrengths[1] == 1.5);
}
END_TEST

START_TEST(sigmoid_output_for_feedforward) {
    //  Arrange
    tissue_initializeDefaultTissue();

    cells_connectDirected(10, 5, 1.0);
    cells_connectDirected(11, 6, 1.5);
    cells_connectDirected(5, 2, 1.0);
    cells_connectDirected(6, 3, 0.3);
    cells_connectDirected(5, 7, 0.5);

    cellTypes_setActivationCalculation(CELL_TYPE_BASIC, activationCalculation_sigmoid);

    //  Act
    int targets[] = {10, 11};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_matrix_feedforward_stim(targets, strengths, count);

    //  Assert
    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 2, "2 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;

    fail_unless(destIndexes[0] == 5);
    fail_unless(destIndexes[1] == 6);

    double s_2_toNearest2Dec = floor(destStrengths[0]*100) / 100;
    double s_7_toNearest2Dec = floor(destStrengths[1]*100) / 100;

    fail_unless(s_2_toNearest2Dec == 0.73, "S_2 = %f, expected %f", s_2_toNearest2Dec, 0.73);
    fail_unless(s_7_toNearest2Dec == .81, "S_2 = %f, expected %f", s_7_toNearest2Dec, 0.81);
}
END_TEST

START_TEST(reset_tissue_state) {

    tissue_initializeDefaultTissue();

    cells_connectDirected(10, 11, 0.5);
    cells_connectDirected(10, 12, 0.1);
    cells_connectDirected(11, 13, 1.0);

    int targets[] = {10, 11};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_stimulate(targets, strengths, count);

    tissue_state_reset();

    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 0, "State should be reset");
}
END_TEST

/**
 * Add these tests to the test suite
 */ 
void cell_communications_addToSuite(Suite *suite) {

    TCase *cellCommunication = tcase_create("Cell Communication");

    tcase_add_test(cellCommunication, message_raw_data_to_dest_cells);
    tcase_add_test(cellCommunication, message_propagate_data_to_cells);
    tcase_add_test(cellCommunication, reset_tissue_state);
    tcase_add_test(cellCommunication, feedfoward_stimulation);
    tcase_add_test(cellCommunication, feedfoward_stimulation_multilayer);
    tcase_add_test(cellCommunication, sigmoid_output_for_feedforward);

    suite_add_tcase(suite, cellCommunication);

}