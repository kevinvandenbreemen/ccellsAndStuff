#ifndef TISSUE_IN
#define TISSUE_IN

#define NUM_CELLS 100

#include "cellTypes.h"
#include "logger.h"
#include "collections.h"

static int chkTissueCreated();
int * getTissue();

/**
 * Current state of the tissue in terms of what cells are lit up/stimulated
 */
typedef struct TissueState {

    int * outputIndices;
    double * outputStrengths;
    int outputCount;

} TissueState;

static TissueState * _getState() {
    static int initd = 0;

    static TissueState state;
    if(initd == 0) {
        state.outputCount = 0;
        state.outputIndices = malloc(sizeof(int));
        state.outputStrengths = malloc(sizeof(double));
        initd = 1;
    }
    

    return &state;
}

TissueState tissue_getState() {
    
    TissueState *currentState = _getState();
    TissueState ret;

    ret.outputCount = currentState->outputCount;
    
    #ifndef NDEBUG
    printf("TISSUE STATE:\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~\n");
    printf("Count = %d\n", ret.outputCount);
    printf("=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~\n");
    #endif

    return ret;

}

tissue_state_updateOutputToCell(int index, double strength) {
    
    TissueState * state = _getState();
    state->outputCount++;

    intArray_addAddOneMoreItem(state->outputIndices, state->outputCount);
    doubleArray_addOneMoreItem(state->outputStrengths, state->outputCount);

    printf("Added output cell idx=%d, stren=%f\n", index, strength);
}

static int chkTissueCreated() {
    static int created = 0;

    int ret = created;
    if(created == 0){
        created = 1;
    }

    return ret;
}

int * getTissue() {
   static int tissueData[NUM_CELLS];
   if(chkTissueCreated() == 0) {
       logD("Creating tissue...");
       int idx = 0;
       for(idx=0; idx < NUM_CELLS; idx++) {
           tissueData[idx] = CELL_TYPE_BASIC;
       }
   } else {
       logD("Tissue already created");
   }

   return tissueData;

}

#endif