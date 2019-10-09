#ifndef IN_COLLECTIONS
#define IN_COLLECTIONS

#include <stdlib.h>

//  Int Arrays
void  intArray_addAddOneMoreItem(int * array, int size);
void  intArray_addAddOneMoreItem(int * array, int size) {

    int * temp = malloc(sizeof(int) * (size+1));
    
    int i;
    for(i=0; i<size; i++) {
        temp[i] = array[i];
    }

    array = temp;

}

//  Double Arrays
void doubleArray_addOneMoreItem(double * array, int size);
void doubleArray_addOneMoreItem(double * array, int size) {
    double * temp = malloc(sizeof(double) * (size+1));

    int i;
    for(i=0; i<size; i++) {
        temp[i] = array[i];
    }

    array = temp;
}

#endif