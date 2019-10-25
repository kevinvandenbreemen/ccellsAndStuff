#ifndef IN_BITARRAY
#define IN_BITARRAY

#define BIT_ON 1
#define BIT_OFF 0
#define INT_SIZE_IN_BITS 32

static const int on = BIT_ON;
static const int off = BIT_OFF;

typedef struct BitArray {
    unsigned int * data;
} BitArray;

/**
 * Create a new BitArray with the given size.  Please be sure to call bitarray_destroy() when 
 * you are done with it.
 */
BitArray * bitarray_create(int size);

/**
 * Frees up memory used by the given BitArray
 */
void bitarray_destroy(BitArray *bitArray);
int bitarray_valueOf(BitArray *bitArray, int position);
void bitarray_writeBit(BitArray *bitArray, int position, int value);

/**
 * Count number of bits turned on in the first n positions
 */
int bitarray_countOn(BitArray *bitArray, int inFirstNPositions);

/**
 * Gets specific indexes that are on in the first N positions
 */
int * bitarray_indexesTurnedOn(BitArray *bitArray, int inFirstNPositions);

BitArray * bitarray_create(int size) {
    BitArray * ret = (BitArray *) malloc(sizeof(BitArray));
    
    //  Compute total ints required to cover the bits
    int numIntsReqd = (size / INT_SIZE_IN_BITS) + 1;

    #ifndef NDEBUG
    printf("Allocating bit array of %d ints for capacity of %d bits\n", numIntsReqd, size);
    #endif

    ret->data = malloc(sizeof(unsigned int) * numIntsReqd);
    int i=0;
    for(i=0; i<numIntsReqd; i++) {
        ret->data[i] = 0;
    }

    return ret;
}

void bitarray_destroy(BitArray *bitArray) {

    if(bitArray == NULL) {
        #ifndef NDEBUG
        printf("Cannot destroy null bit array.  Taking no action\n");
        #endif
        return;
    }

    free(bitArray->data);
    free(bitArray);
}

int bitarray_valueOf(BitArray *bitArray, int position) {

    int index = position / INT_SIZE_IN_BITS;
    int positionInInt = position % INT_SIZE_IN_BITS;

    #ifndef NDEBUG
    printf("(bitarray at pos %d):  Interrogating integer at pos %d, raw=%d for bit at %d\n", position, index, bitArray->data[index], positionInInt);
    #endif

    return ((bitArray->data[index] & (1 << positionInInt))) != 0 ? on : off;

}

int bitarray_countOn(BitArray *bitArray, int inFirstNPositions) {
    int count = 0;
    int i;
    for(i=0; i<inFirstNPositions; i++) {
        if(bitarray_valueOf(bitArray, i) == on) {
            printf("[BITARRAY]  Found VAL at %d\n", i);
            count ++;
        }
    }

    printf("[BITARRAY]  total on=%d\n", count);

    return count;
}

int * bitarray_indexesTurnedOn(BitArray *bitArray, int inFirstNPositions) {
    int numOn = bitarray_countOn(bitArray, inFirstNPositions);
    int i;
    int index=0;
    int * ret = malloc(numOn * sizeof(int));
    for(i=0; i<inFirstNPositions; i++) {
        if(bitarray_valueOf(bitArray, i) == on) {
            ret[index++] = i;
        }
    }

    return ret;
}

void bitarray_writeBit(BitArray *bitArray, int position, int value) {

    int index = position / INT_SIZE_IN_BITS;
    int positionInInt = position % INT_SIZE_IN_BITS;

    unsigned int flagToWrite = 1;
    flagToWrite = flagToWrite << positionInInt;

    bitArray->data[index] = bitArray->data[index] | flagToWrite; 

}

#endif