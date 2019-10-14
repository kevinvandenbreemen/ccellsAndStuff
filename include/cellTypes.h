#ifndef CELLTYPES_IN
#define CELLTYPES_IN

#include <stdlib.h>
#include "cellTypeFunctions.h"

#define MAX_CELL_TYPES 128  //  Maximum number of cell types that can be defined in the system

#define CELL_TYPE_BASIC 1
#define CELL_TYPE_INHIB 2

/**
 * Behaviour of a given cell type
 */
typedef struct CellTypeBehaviour {

    double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength);

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

}

void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength));
void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(double inputStrength, double outgoingConnectionStrength)) {
    (cellTypeBehaviours[cellType]).getOutputStrength = getOutputStrength;
}

CellTypeBehaviour *cellTypes_behaviourFor(int cellType) {
    return &cellTypeBehaviours[cellType];
}


#endif