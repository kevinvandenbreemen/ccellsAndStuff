#include "include/integration.h"
#include <stdio.h>

int majorVersion() {
    printf("Trying to get Version\n");
    return MAJ_VER;
}

int minorVersion() {
    return MIN_VER;
}
int revisionNum() {
    return REV_VER;
}

void swift_tissue_initializeDefaultTissue() {
    tissue_initializeDefaultTissue();
}

int swift_cells_connectDirected(int from, int to, double strength) {
    return cells_connectDirected(from, to, strength);
}

//  Cell Types
//  -----------------------------------------------
int swift_tissue_getCellType(int index) {
    return tissue_getCellType(index);
}
void swift_cellTypes_setBehaviourForCellType(int cellType, CellTypeBehaviour *behaviour) {
    cellTypes_setOutputStrengthCalc(cellType, behaviour->getOutputStrength);
    cellTypes_setCellLogicForIncomingConnections(cellType, behaviour->processIncomingConnections);
    cellTypes_setCellLogicForOutgoingConnections(cellType, behaviour->processOutgoingConnections);
    cellTypes_setCellBehaviourLogic(cellType, behaviour->executeCellBehaviour);
}

void swift_tissue_setCellType(int index, int cellType) {
    tissue_setCellType(index, cellType);
}

//  Network stimulation
//  -----------------------------------------------
void swift_cells_stimulate(int * targets, double * strengths, int count) {
    cells_stimulate(targets, strengths, count);
}

void swift_cells_matrix_feedfoward_stim(int * targets, double * strengths, int count) {
    cells_matrix_feedforward_stim(targets, strengths, count);
}

//  Driver
void swift_tissue_resetAll() {
    tissue_resetAll();
}

void swift_tissue_setOnStateUpdate(TissueStateCallback *callback) {
    tissue_setOnStateUpdate(callback->onStateUpdate);
}

int swift_tissue_getNumCells() {
    return tissue_getNumCells();
}

void swift_tissue_executeCellBehaviours() {
    tissue_executeCellBehaviours();
}

//  Cell connections
CellConnectionInfo * swift_cells_getConnectedFrom(int cellIndex) {
    int numConnections = cells_countConnectedFrom(cellIndex);
    int i;

    CellConnectionInfo * ret = malloc(sizeof(CellConnectionInfo));

    ret->numConnections = numConnections;
    ret->cellIndexes = cells_indexesOfConnectedFrom(cellIndex);
    ret->connectionStrengths = malloc(sizeof(double) * numConnections);
    
    for(i=0; i<numConnections; i++) {
        ret->connectionStrengths[i] = cells_strengthOfConnection(cellIndex, ret->cellIndexes[i]);
    }

    return ret;
}

CellConnectionInfo * swift_cells_connectedTo(int cellIndex) {
    int numConnections = cells_countConnectedTo(cellIndex);
    int i;

    CellConnectionInfo * ret = malloc(sizeof(CellConnectionInfo));

    ret->numConnections = numConnections;
    ret->cellIndexes = cells_indexesOfConnectedTo(cellIndex);
    ret->connectionStrengths = malloc(sizeof(double) * numConnections);
    
    for(i=0; i<numConnections; i++) {
        ret->connectionStrengths[i] = cells_strengthOfConnection(ret->cellIndexes[i], cellIndex);
    }

    return ret;
}

//  Display of tissues
//  -----------------------------------------------
void swift_tissue_display_tissue() {
    tissue_display_tissue();
}

void swift_cellTypes_setColor(int cellType, CellTypeColor color) {
    cellTypes_setColor(cellType, color);
}

CellTypeColor swift_cellTypes_getColor(int cellType) {
    return cellTypes_getColor(cellType);
}