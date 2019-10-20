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

BitArray * bitarray_create(int size);
int bitarray_valueOf(BitArray *bitArray, int position);
void bitarray_writeBit(BitArray *bitArray, int position, int value);

BitArray * bitarray_create(int size) {
    BitArray * ret = (BitArray *) malloc(sizeof(BitArray));
    
    //  Compute total ints required to cover the bits
    int numIntsReqd = size / INT_SIZE_IN_BITS;
    ret->data = malloc(sizeof(unsigned int) * numIntsReqd);
    int i=0;
    for(i=0; i<numIntsReqd; i++) {
        ret->data[i] = 0;
    }

    return ret;
}

int bitarray_valueOf(BitArray *bitArray, int position) {

    int index = position / INT_SIZE_IN_BITS;
    int positionInInt = position % INT_SIZE_IN_BITS;

    #ifndef NDEBUG
    printf("(bitarray at pos %d):  Interrogating integer at pos %d, raw=%d for bit at %d\n", position, index, bitArray->data[index], positionInInt);
    #endif

    return ((bitArray->data[index] & (1 << positionInInt))) != 0 ? on : off;

}

void bitarray_writeBit(BitArray *bitArray, int position, int value) {

    int index = position / INT_SIZE_IN_BITS;
    int positionInInt = position % INT_SIZE_IN_BITS;

    unsigned int flagToWrite = 1;
    flagToWrite = flagToWrite << positionInInt;

    bitArray->data[index] = bitArray->data[index] | flagToWrite; 

}

#endif