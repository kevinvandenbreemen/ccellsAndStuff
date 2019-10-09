#include <check.h>
#include "../include/msgs.h"
#include "../include/tissue.h"

START_TEST(message_raw_data_to_dest_cells) {

    cells_connectDirected(10, 11, 0.5);
    cells_connectDirected(10, 12, 0.1);

    double * output = cells_rawOutput(10, 2.0);
    
    fail_unless(output[0] == 1.0, "Output for for (10->11)[strength=0.5] for input 2.0 should be 1.0 but was %f", output[0]);
    fail_unless(output[1] == 0.2, "Output for for (10->12)[strength=0.1] for input 2.0 should be 0.2 but was %f", output[1]);

}
END_TEST

START_TEST(message_propagate_data_to_cells) {

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

/**
 * Add these tests to the test suite
 */ 
void cell_communications_addToSuite(Suite *suite) {

    TCase *cellCommunication = tcase_create("Cell Communication");

    tcase_add_test(cellCommunication, message_raw_data_to_dest_cells);
    tcase_add_test(cellCommunication, message_propagate_data_to_cells);

    suite_add_tcase(suite, cellCommunication);

}