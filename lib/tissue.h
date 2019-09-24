#ifndef TISSUE_IN
#define TISSUE_IN

#include "cellTypes.h"
#include "logger.h"

static int chkTissueCreated() {
    static int created = 0;

    int ret = created;
    if(created == 0){
        created = 1;
    }

    return ret;
}

int * getTissue() {
   static int tissueData[100];
   if(chkTissueCreated() == 0) {
       logD("Creating tissue...");
       int idx = 0;
       for(idx=0; idx < sizeof(tissueData); idx++) {
           tissueData[idx] = CELL_TYPE_BASIC;
       }
   } else {
       logD("Tissue already created");
   }

   return tissueData;

}

#endif