#ifndef IN_DRIVER
#define IN_DRIVER

//  Main system integration driver.
//  Do not put any application logic in this file.  It is for performing tasks
//  that require a full knowledge of all subsystems.

/**
 * Resets the tissue back to all basic cell types, no connections, clearing the state and any call type functions
 * or other callbacks.  This function "resets the world".  Use with caution.
 */
void tissue_resetAll();
void tissue_resetAll(){

    int * tissue = getTissue();

    int i;
    for(i=0; i<NUM_CELLS; i++) {
        tissue[i] = CELL_TYPE_BASIC;
    }

    cells_resetConnections();
    cellTypes_AllocateCellTypeBehaviours();
}

#endif