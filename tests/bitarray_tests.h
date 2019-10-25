#include <check.h>
#include "../include/bitarray.h"

START_TEST(write_position) {

    BitArray *bitArray = bitarray_create(150);

    bitarray_writeBit(bitArray, 12, 1);

    fail_unless(bitarray_valueOf(bitArray, 12) == 1);
    fail_unless(bitarray_valueOf(bitArray, 0) == 0);

}
END_TEST

START_TEST(write_position_beyond_bound_of_int) {

    BitArray *bitArray = bitarray_create(1024);

    bitarray_writeBit(bitArray, 429, 1);

    fail_unless(bitarray_valueOf(bitArray, 429) == 1);
    fail_unless(bitarray_valueOf(bitArray, 144) == 0, "Value at 144 should be 0 but was %d", bitarray_valueOf(bitArray, 144));

}
END_TEST

START_TEST(write_same_position_twice) {

    BitArray *bitArray = bitarray_create(1024);

    bitarray_writeBit(bitArray, 429, 1);
    bitarray_writeBit(bitArray, 429, 1);
    bitarray_writeBit(bitArray, 430, 1);

    fail_unless(bitarray_valueOf(bitArray, 429) == 1);
    fail_unless(bitarray_valueOf(bitArray, 430) == 1);
    fail_unless(bitarray_valueOf(bitArray, 144) == 0, "Value at 144 should be 0 but was %d", bitarray_valueOf(bitArray, 144));

}
END_TEST

START_TEST(count_number_on) {
    BitArray *bitArray = bitarray_create(NUM_CELLS);
    bitarray_writeBit(bitArray, 3, on);
    bitarray_writeBit(bitArray, 4, on);
    bitarray_writeBit(bitArray, 3, on);

    fail_unless(bitarray_countOn(bitArray, NUM_CELLS) == 2);
}
END_TEST

START_TEST(indexes_turned_on) {
    BitArray *bitArray = bitarray_create(NUM_CELLS);
    bitarray_writeBit(bitArray, 3, on);
    bitarray_writeBit(bitArray, 4, on);
    bitarray_writeBit(bitArray, 3, on);

    int numOn = bitarray_countOn(bitArray, NUM_CELLS);
    int * indexesOn = bitarray_indexesTurnedOn(bitArray, NUM_CELLS);

    fail_unless(indexesOn[0] == 3, "FIrst index should be 3 but was %d", indexesOn[0] );
    fail_unless(indexesOn[1] == 4, "Second inex should be 4 but was %d", indexesOn[1] );
}
END_TEST

void bitarray_tests_addToSuite(Suite *suite) {
    TCase *bitarrayTests = tcase_create("Bit Array Tests");

    tcase_add_test(bitarrayTests, write_position);
    tcase_add_test(bitarrayTests, write_position_beyond_bound_of_int);
    tcase_add_test(bitarrayTests, write_same_position_twice);
    tcase_add_test(bitarrayTests, count_number_on);
    tcase_add_test(bitarrayTests, indexes_turned_on);

    suite_add_tcase(suite, bitarrayTests);

}