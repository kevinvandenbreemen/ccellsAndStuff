#ifndef CELLTYPES_IN
#define CELLTYPES_IN

#include <stdlib.h>

#define MAX_CELL_TYPES 128  //  Maximum number of cell types that can be defined in the system

#define CELL_TYPE_BASIC 1
#define CELL_TYPE_INHIB 2

static BitArray *cellTypesWithProcessIncomingConnections;
static BitArray *cellTypesWithProcessOutgoingConnections;

static void reinitCellTypesProcessingMaps() {
    bitarray_destroy(cellTypesWithProcessIncomingConnections);
    bitarray_destroy(cellTypesWithProcessOutgoingConnections);

    cellTypesWithProcessIncomingConnections = bitarray_create(MAX_CELL_TYPES);
    cellTypesWithProcessOutgoingConnections = bitarray_create(MAX_CELL_TYPES);
}

/**
 * Behaviour of a given cell type
 */
typedef struct CellTypeBehaviour {

    double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength);

    void (*processIncomingConnections)(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths);

    void (*processOutgoingConnections)(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths);

} CellTypeBehaviour;

CellTypeBehaviour *cellTypeBehaviours;

/**
 * Allocates or resets the cellTypeBehaviours for the system.  This method will wipe out any previously existing cell
 * type behaviours so use with caution!
 */
void cellTypes_AllocateCellTypeBehaviours();
void cellTypes_AllocateCellTypeBehaviours() {
    free(cellTypeBehaviours);
    cellTypeBehaviours =  malloc(sizeof(CellTypeBehaviour) * MAX_CELL_TYPES);

    reinitCellTypesProcessingMaps();
}

void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength));
void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength)) {
    (cellTypeBehaviours[cellType]).getOutputStrength = getOutputStrength;
}

//  Configure default cell type behaviours
void cellTypes_InitializeDefaultCellTypeBehaviours();
void cellTypes_InitializeDefaultCellTypeBehaviours() {
    cellTypes_AllocateCellTypeBehaviours();

    //  Basic Cell Type
    cellTypes_setOutputStrengthCalc(CELL_TYPE_BASIC, *basic_calculateOutputStrength);

    //  Inhibitory cell type
    cellTypes_setOutputStrengthCalc(CELL_TYPE_INHIB, *inhibitory_calculateOutputStrength);

}



void cellTypes_setCellLogicForIncomingConnections(int cellType, void (*logic)(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths));
void cellTypes_setCellLogicForIncomingConnections(int cellType, void (*logic)(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths)){
    (cellTypeBehaviours[cellType]).processIncomingConnections = logic;
    bitarray_writeBit(cellTypesWithProcessIncomingConnections, cellType, 1);
}

int cellTypes_existsLogicForIncomingConnectionsForCellType(int cellType);
int cellTypes_existsLogicForIncomingConnectionsForCellType(int cellType) {
    if(bitarray_valueOf(cellTypesWithProcessIncomingConnections, cellType) == on){
        return 1;
    }
    return 0;
}

void cellTypes_setCellLogicForOutgoingConnections(int cellType, void (*logic)(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths));
void cellTypes_setCellLogicForOutgoingConnections(int cellType, void (*logic)(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths)) {
    (cellTypeBehaviours[cellType]).processOutgoingConnections = logic;
    bitarray_writeBit(cellTypesWithProcessOutgoingConnections, cellType, 1);
}
int cellTypes_existsLogicForOutgoingConnectionsForCellType(int cellType);
int cellTypes_existsLogicForOutgoingConnectionsForCellType(int cellType) {
    if(bitarray_valueOf(cellTypesWithProcessOutgoingConnections, cellType) == on){
        return 1;
    }
    return 0;
}


CellTypeBehaviour *cellTypes_behaviourFor(int cellType) {
    return &cellTypeBehaviours[cellType];
}


#endif