#ifndef IN_TISSUE_TESTS
#define IN_TISSUE_TESTS

#include <check.h>
#include "../include/tissue.h"
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
    double outputStrength = basicCellTypeBehaviour->getOutputStrength(0.5, 0.5);
    fail_unless(outputStrength == 0.5, "Output Strength should be base input strength * output connection strength");

    basicCellTypeBehaviour = cellTypes_behaviourFor(CELL_TYPE_INHIB);
    fail_if(basicCellTypeBehaviour == NULL, "System should have initialized 'inhibitory' cell type behaviour");

}
END_TEST

void tissue_tests_addToSuite(Suite *suite) {
    TCase *tissueTests = tcase_create("Tissue/Cell Type Tests");

    tcase_add_test(tissueTests, set_cell_type);
    tcase_add_test(tissueTests, cell_type_out_of_bounds);
    tcase_add_test(tissueTests, initialize_cell_type_behaviours);
    tcase_add_test(tissueTests, initialize_default_cell_type_behaviours);
    tcase_add_test(tissueTests, get_default_cell_type_behaviours);

    suite_add_tcase(suite, tissueTests);
}

#endif