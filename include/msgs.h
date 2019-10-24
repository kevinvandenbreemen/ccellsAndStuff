#ifndef KSIGNALS_IN
#define KSIGNALS_IN


double * cells_rawOutput(int inputCellIndex, double inputStrength);

/**
 * Stimulate the tissue at the given target cells with the given strengths.  Count is passed as a 
 * convenience to cut down on needing to work out array lengths.
 */
void cells_stimulate(int * targets, double * strengths, int count);

double * cells_rawOutput(int inputCellIndex, double inputStrength) {

    int size = cells_countConnectedFrom(inputCellIndex);
    int * endpointIndexes = cells_indexesOfConnectedFrom(inputCellIndex);
    double connectionStrengths[size];
    
    int i;
    for(i=0; i<size; i++) {
        connectionStrengths[i] = cells_strengthOfConnection(inputCellIndex, endpointIndexes[i]);
    }

    free(endpointIndexes);

    double * ret = kmath_linalg_scalar_times_vec(connectionStrengths, size, inputStrength);

    CellTypeBehaviour * behaviour = cellTypes_behaviourFor(tissue_getCellType(inputCellIndex));

    for(i=0; i<size; i++) {
        ret[i] = behaviour->getOutputStrength(inputStrength, ret[i]);
    }

    return ret;

}

static void doCellBehaviourAndLogic(int cellIndex) {

    CellTypeBehaviour * behaviour = cellTypes_behaviourFor(tissue_getCellType(cellIndex));
    if(behaviour == NULL) {
        fprintf(stderr, "Cell behaviour for cell type %d not initialized.  Perhaps you have configured the system incorrectly\n", tissue_getCellType(cellIndex));
        return;
    }

    //  Incoming connections logic
    if(cellTypes_existsLogicForIncomingConnectionsForCellType(tissue_getCellType(cellIndex))) {
        int count = cells_countConnectedTo(cellIndex);
        if(count > 0){
            int * indexesOfIncomingConnections = cells_indexesOfConnectedTo(cellIndex);
            
            double * strengths = malloc(count * sizeof(double));
            int i;
            for(i=0; i<count; i++) {
                strengths[i] = cells_strengthOfConnection(indexesOfIncomingConnections[i], cellIndex);
            }

            behaviour->processIncomingConnections(count, cellIndex, indexesOfIncomingConnections, strengths);

            free(indexesOfIncomingConnections);
            free(strengths);
        } else {
            #ifndef NDEBUG
            printf("Cell at %d has no incoming connections -- no action required\n", cellIndex);
            #endif
        }
    } else {
        #ifndef NDEBUG
        printf("No logic to handle incoming connections for cell type %d\n", tissue_getCellType(cellIndex));
        #endif
    }

    //  Outgoing connections logic
    if(cellTypes_existsLogicForOutgoingConnectionsForCellType(tissue_getCellType(cellIndex))) {

        int count = cells_countConnectedFrom(cellIndex);
        if(count > 0) {
            int * indexesOfOutgoingConnections = cells_indexesOfConnectedFrom(cellIndex);
            
            double * strengths = malloc(count * sizeof(double));
            int i;
            for(i=0; i<count; i++) {
                strengths[i] = cells_strengthOfConnection(cellIndex, indexesOfOutgoingConnections[i]);
            }

            behaviour->processOutgoingConnections(count, cellIndex, indexesOfOutgoingConnections, strengths);

            free(indexesOfOutgoingConnections);
            free(strengths);

        } else{
            #ifndef NDEBUG
            printf("Cell at %d has no outgoing connections -- no action required\n", cellIndex);
            #endif
        }
    } else {
        #ifndef NDEBUG
        printf("No logic to handle outgoing connections for cell type %d\n", tissue_getCellType(cellIndex));
        #endif
    }
    
}

static void doStimulateCell(int inputCellIndex, double inputStrength, BitArray *touchedCellIndexes) {

    bitarray_writeBit(touchedCellIndexes, inputCellIndex, 1);

    int connectionCount = cells_countConnectedFrom(inputCellIndex);
    int * endpointIndexes = cells_indexesOfConnectedFrom(inputCellIndex);

    if(endpointIndexes == NULL) {
        tissue_state_updateOutputToCell(inputCellIndex, inputStrength);
        return;
    }

    double * outputs = cells_rawOutput(inputCellIndex, inputStrength);

    int i;
    for(i=0; i<connectionCount; i++) {
        doStimulateCell(endpointIndexes[i], outputs[i], touchedCellIndexes);
    }

    free(outputs);
    free(endpointIndexes);
}



/**
 * Send information into the network at the given targets with the given input strengths.
 */
void cells_stimulate(int * targets, double * strengths, int count);
void cells_stimulate(int * targets, double * strengths, int count) {
    
    BitArray *touchedCellIndexes = bitarray_create(NUM_CELLS);

    int i;
    for(i=0; i<count; i++) {
        
        #ifndef NDEBUG
        printf("TARGET %d ----\n", targets[i]);
        #endif

        doStimulateCell(targets[i], strengths[i], touchedCellIndexes);

        #ifndef NDEBUG
        printf("TARGET %d DONE\n", targets[i]);
        #endif
    }

    for(i=0; i<NUM_CELLS; i++) {
        if(bitarray_valueOf(touchedCellIndexes, i) == on) {
            doCellBehaviourAndLogic(i);
        }
    }

    bitarray_destroy(touchedCellIndexes);

}



#endif