#ifndef CELLTYPES_IN
#define CELLTYPES_IN

#include <stdlib.h>
#include "cellTypeFunctions.h"

#define MAX_CELL_TYPES 128  //  Maximum number of cell types that can be defined in the system

#define CELL_TYPE_BASIC 1
#define CELL_TYPE_INHIB 2

static int cellTypesWithProcessIncomingConnections[MAX_CELL_TYPES] = {0};
static int cellTypesWithProcessOutgoingConnections[MAX_CELL_TYPES] = {0};

/**
 * Behaviour of a given cell type
 */
typedef struct CellTypeBehaviour {

    double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength);

    void (*processIncomingConnections)(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths);

    void (*processOutgoingConnections)(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths);

} CellTypeBehaviour;

CellTypeBehaviour *cellTypeBehaviours;
void cellTypes_AllocateCellTypeBehaviours();
void cellTypes_AllocateCellTypeBehaviours() {
    free(cellTypeBehaviours);
    cellTypeBehaviours =  malloc(sizeof(CellTypeBehaviour) * MAX_CELL_TYPES);
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

void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength));
void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength)) {
    (cellTypeBehaviours[cellType]).getOutputStrength = getOutputStrength;
}

void cellTypes_setCellLogicForIncomingConnections(int cellType, void (*logic)(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths));
void cellTypes_setCellLogicForIncomingConnections(int cellType, void (*logic)(int size, int cellIndex, int * incomingIndexes, double * incomingStrengths)){
    (cellTypeBehaviours[cellType]).processIncomingConnections = logic;
    cellTypesWithProcessIncomingConnections[cellType] = 1;
}

int cellTypes_existsLogicForIncomingConnectionsForCellType(int cellType);
int cellTypes_existsLogicForIncomingConnectionsForCellType(int cellType) {
    if(cellTypesWithProcessIncomingConnections[cellType] == 1){
        return 1;
    }
    return 0;
}

void cellTypes_setCellLogicForOutgoingConnections(int cellType, void (*logic)(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths));
void cellTypes_setCellLogicForOutgoingConnections(int cellType, void (*logic)(int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths)) {
    (cellTypeBehaviours[cellType]).processOutgoingConnections = logic;
    cellTypesWithProcessOutgoingConnections[cellType] = 1;
}
int cellTypes_existsLogicForOutgoingConnectionsForCellType(int cellType);
int cellTypes_existsLogicForOutgoingConnectionsForCellType(int cellType) {
    if(cellTypesWithProcessOutgoingConnections[cellType] == 1){
        return 1;
    }
    return 0;
}


CellTypeBehaviour *cellTypes_behaviourFor(int cellType) {
    return &cellTypeBehaviours[cellType];
}


#endif