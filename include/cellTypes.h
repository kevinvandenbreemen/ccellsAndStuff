#ifndef CELLTYPES_IN
#define CELLTYPES_IN

#include <stdlib.h>

#define MAX_CELL_TYPES 128  //  Maximum number of cell types that can be defined in the system

#define CELL_TYPE_BASIC 1
#define CELL_TYPE_INHIB 2

static BitArray *cellTypesWithProcessIncomingConnections;
static BitArray *cellTypesWithProcessOutgoingConnections;
static BitArray *cellTypesWithStrengthCalculation;
static BitArray *cellTypesWithActivationFunctions;
static BitArray *cellTypesWithBehaviourFunctions;
static BitArray *cellTypesWithCellTypeColors;

static void reinitCellTypesProcessingMaps() {
    bitarray_destroy(cellTypesWithProcessIncomingConnections);
    bitarray_destroy(cellTypesWithProcessOutgoingConnections);
    bitarray_destroy(cellTypesWithStrengthCalculation);
    bitarray_destroy(cellTypesWithActivationFunctions);
    bitarray_destroy(cellTypesWithBehaviourFunctions);

    cellTypesWithProcessIncomingConnections = bitarray_create(MAX_CELL_TYPES);
    cellTypesWithProcessOutgoingConnections = bitarray_create(MAX_CELL_TYPES);
    cellTypesWithStrengthCalculation = bitarray_create(MAX_CELL_TYPES);
    cellTypesWithActivationFunctions = bitarray_create(MAX_CELL_TYPES);
    cellTypesWithBehaviourFunctions = bitarray_create(MAX_CELL_TYPES);
}

//  Cell Type Colors
//  ----------------------------------------------------------------------
/**
 * RGB color for drawing a cell of this type.  Also includes an integer alpha, which provides value from 
 * 0 to 255
 */
typedef struct CellTypeColor {
    int red;
    int green;
    int blue;
    int alpha;
} CellTypeColor;

static CellTypeColor cellTypes_defaultCellTypeColor;
static CellTypeColor *cellTypeColors;

static void initializeCellTypeColors() {
    cellTypes_defaultCellTypeColor.red = 255;
    cellTypes_defaultCellTypeColor.green = 0;
    cellTypes_defaultCellTypeColor.blue = 0;
    cellTypes_defaultCellTypeColor.alpha = 100;

    free(cellTypeColors);
    cellTypeColors = malloc(sizeof(CellTypeColor) * MAX_CELL_TYPES);
    
    bitarray_destroy(cellTypesWithCellTypeColors);
    cellTypesWithCellTypeColors = bitarray_create(MAX_CELL_TYPES);
}

void cellTypes_setColor(int cellType, CellTypeColor color);
void cellTypes_setColor(int cellType, CellTypeColor color) {
    *(cellTypeColors + cellType) = color;
    bitarray_writeBit(cellTypesWithCellTypeColors, cellType, 1);
}

CellTypeColor cellTypes_getColor(int cellType);
CellTypeColor cellTypes_getColor(int cellType) {
    if(bitarray_valueOf(cellTypesWithCellTypeColors, cellType) == on) {
        return *(cellTypeColors + cellType);
    }

    return cellTypes_defaultCellTypeColor;
}

void cellTypes_initializeCellTypeColors() {
    initializeCellTypeColors();
}

//  End Cell Type Colors
//  ----------------------------------------------------------------------


/**
 * Behaviour of a given cell type
 */
typedef struct CellTypeBehaviour {

    /**
     * Given a single input to this neuron and its calculated output connection strength, determines
     * most appropriate output strength to send to its connected neuron
     */
    double (*getOutputStrength)(int cellType, double inputStrength, double outgoingConnectionStrength);

    /**
     * Cell at the specific index must decide on modifications to make to its incoming connections
     */
    void (*processIncomingConnections)(int cellType, int size, int cellIndex, int * incomingIndexes, double * incomingStrengths);

    /**
     * Cell at specific index must decide on modifications to make to its outgoing connections
     */
    void (*processOutgoingConnections)(int cellType, int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths);

    /**
     * Given a weighted input sum (for example in a feedforward network) calculates the correct activation (to be used as output)
     * for the cell of the given type
     */
    double (*calculateActivation)(int cellType, double weightedInputSum);

    /**
     * Execute any additional general logic for this cell.  Usually you should implement this function in most networks.
     */
    void (*executeCellBehaviour)(int cellType, int index);

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

void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(int cellType, double inputStrength, double outgoingConnectionStrength));
void cellTypes_setOutputStrengthCalc(int cellType, double (*getOutputStrength)(int cellType, double inputStrength, double outgoingConnectionStrength)) {
    (cellTypeBehaviours[cellType]).getOutputStrength = getOutputStrength;
    bitarray_writeBit(cellTypesWithStrengthCalculation, cellType, on);
}

void cellTypes_setActivationCalculation(int cellType, double (*activationFunction)(int cellType, double weightInputSum));
void cellTypes_setActivationCalculation(int cellType, double (*activationFunction)(int cellType, double weightInputSum)) {
    (cellTypeBehaviours[cellType]).calculateActivation = activationFunction;
    bitarray_writeBit(cellTypesWithActivationFunctions, cellType, on);
}

/**
 * Set logic for cells of the given type.  Generally you should put most of your cell logic in this function and 
 * then use tissue_executeCellBehaviours() to drive most cell logic.
 */
void cellTypes_setCellBehaviourLogic(int cellType, void (*behaviourFunction)(int cellType, int cellIndex));
void cellTypes_setCellBehaviourLogic(int cellType, void (*behaviourFunction)(int cellType, int cellIndex)) {
    (cellTypeBehaviours[cellType]).executeCellBehaviour = behaviourFunction;
    bitarray_writeBit(cellTypesWithBehaviourFunctions, cellType, on);
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



void cellTypes_setCellLogicForIncomingConnections(int cellType, void (*logic)(int cellType, int size, int cellIndex, int * incomingIndexes, double * incomingStrengths));
void cellTypes_setCellLogicForIncomingConnections(int cellType, void (*logic)(int cellType, int size, int cellIndex, int * incomingIndexes, double * incomingStrengths)){
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

int cellTypes_existsLogicForStrengthDetermination(int cellType);
int cellTypes_existsLogicForStrengthDetermination(int cellType) {
    if(bitarray_valueOf(cellTypesWithStrengthCalculation, cellType) == on){
        return 1;
    }
    return 0;
}

int cellTypes_existsActivationFunction(int cellType);
int cellTypes_existsActivationFunction(int cellType) {
    if(bitarray_valueOf(cellTypesWithActivationFunctions, cellType) == on) {
        return 1;
    }
    return 0;
}

int cellTypes_existsBehaviourForCellType(int cellType);
int cellTypes_existsBehaviourForCellType(int cellType) {
    if(bitarray_valueOf(cellTypesWithBehaviourFunctions, cellType) == on) {
        return 1;
    }
    return 0;
}

void cellTypes_setCellLogicForOutgoingConnections(int cellType, void (*logic)(int cellType, int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths));
void cellTypes_setCellLogicForOutgoingConnections(int cellType, void (*logic)(int cellType, int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths)) {
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