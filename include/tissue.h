#ifndef TISSUE_IN
#define TISSUE_IN

#define NUM_CELLS 100

#include "cellTypes.h"
#include "logger.h"

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
    static TissueState state;
    return &state;
}

TissueState tissue_getState() {
    
    
    TissueState ret;


    return ret;

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