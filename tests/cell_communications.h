#include <check.h>
#include "../lib/msgs.h"

START_TEST(message_raw_data_to_dest_cells) {

    cells_connectDirected(10, 11, 0.5);
    cells_connectDirected(10, 12, 0.1);

    double * output = cells_rawOutput(10, 2.0);
    
    fail_unless(*output == 1.0, "Output for for (10->11)[strength=0.5] for input 2.0 should be 1.0");
    fail_unless(*(output + 1) == 0.2, "Output for for (10->12)[strength=0.1] for input 2.0 should be 0.2");

}
END_TEST

/**
 * Add these tests to the test suite
 */ 
void cell_communications_addToSuite(Suite *suite) {

    TCase *cellCommunication = tcase_create("Cell Communication");

    tcase_add_test(cellCommunication, message_raw_data_to_dest_cells);

    suite_add_tcase(suite, cellCommunication);

}