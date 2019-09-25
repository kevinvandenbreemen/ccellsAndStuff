#include <check.h>

#include "lib/tissue.h"

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

int main(int argc, char const *argv[])
{
    Suite *coreSuite = suite_create("Core");
    
    //  Test Case Definitions
    TCase *tissueCreationTests = tcase_create("Tissue Creation");
    suite_add_tcase(coreSuite, tissueCreationTests);
    tcase_add_test(tissueCreationTests, cell_count);
    tcase_add_test(tissueCreationTests, cell_update);

    SRunner *testRunner = srunner_create(coreSuite);

    int testFailureCount;

    srunner_run_all(testRunner, CK_ENV);
    testFailureCount = srunner_ntests_failed(testRunner);
    srunner_free(testRunner);

    return testFailureCount == 0 ? 0 : 1;
}
