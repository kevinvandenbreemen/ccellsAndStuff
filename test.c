#include <check.h>

#include "include/tissue.h"
#include "include/connections.h"

//  Test includes
#include "tests/cell_communications.h"

START_TEST(cell_count) {

    int * cells = getTissue();
    int elem = cells[NUM_CELLS-1];
    int noSuchElement = cells[NUM_CELLS];

    fail_unless(elem == 1, "System did not created the required number of cells");
    fail_if(noSuchElement == 1, "Max element should be index %d", (NUM_CELLS-1));
}
END_TEST

START_TEST(cell_update) {

    int * cells = getTissue();
    cells[1] = 2;

    int * updatedCells = getTissue();
    int value = updatedCells[1];
    fail_unless(value == 2, "System failed to update cell type");

}
END_TEST

START_TEST(no_outgoing_connections) {   
    int * connected = cells_indexesOfConnectedFrom(2);
    fail_unless(connected == 0, "Should be no cells connected from index 2");
}
END_TEST

START_TEST(connect_cells) {
    cells_connectDirected(0, 1, 1.0);
    fail_unless(cells_countConnectedFrom(0) == 1, "Connection count expected");
}
END_TEST

START_TEST(indexes_of_connected_cells) {
    cells_connectDirected(0, 1, 1.0);
    int * connectedCellIndexes = cells_indexesOfConnectedFrom(0);
    fail_unless( *(connectedCellIndexes) == 1 );
}
END_TEST

START_TEST(bidirectional_connection) {
    cells_connectBidirectional(5, 6, 0.5);
    int * connectedCellIndexes = cells_indexesOfConnectedFrom(5);

    fail_unless( *(connectedCellIndexes) == 6, "Bidirectional connection between indexes 5 and 6 - expected connection from 5 to 6");

    connectedCellIndexes = cells_indexesOfConnectedFrom(6);
    fail_unless( *(connectedCellIndexes) == 5, "Bidirectional connection between indexes 5 and 6 - expected connection from 6 to 5");
}
END_TEST

int main(int argc, char const *argv[])
{
    Suite *coreSuite = suite_create("Core");
    
    //  Test Suite Definitions
    TCase *tissueCreationTests = tcase_create("Tissue Creation");
    suite_add_tcase(coreSuite, tissueCreationTests);

    //  Test Case Definitions
    TCase *cellConnectivityTests = tcase_create("Tissue Connectivity");
    suite_add_tcase(coreSuite, cellConnectivityTests);

    //  Test Case Definitions:
    //  Tissue Creation
    tcase_add_test(tissueCreationTests, cell_count);
    tcase_add_test(tissueCreationTests, cell_update);

    //  Tissue Connection
    tcase_add_test(cellConnectivityTests, no_outgoing_connections);
    tcase_add_test(cellConnectivityTests, connect_cells);
    tcase_add_test(cellConnectivityTests, indexes_of_connected_cells);
    tcase_add_test(cellConnectivityTests, bidirectional_connection);

    //  Tess from includes
    cell_communications_addToSuite(coreSuite);

    SRunner *testRunner = srunner_create(coreSuite);

    int testFailureCount;

    srunner_run_all(testRunner, CK_ENV);
    testFailureCount = srunner_ntests_failed(testRunner);
    srunner_free(testRunner);

    return testFailureCount == 0 ? 0 : 1;
}
