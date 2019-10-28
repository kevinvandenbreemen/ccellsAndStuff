#ifndef KSIGNALS_IN
#define KSIGNALS_IN


double * cells_rawOutput(int inputCellIndex, double inputStrength);

/**
 * Stimulate the tissue at the given target cells with the given strengths.  Count is passed as a 
 * convenience to cut down on needing to work out array lengths.
 */
void cells_stimulate(int * targets, double * strengths, int count);

/**
 * Stimulate the network using feed-forward stimulation via layer connection matrices
 */
void cells_matrix_feedforward_stim(int * targets, double * strengths, int count);

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

    if(cellTypes_existsLogicForStrengthDetermination(tissue_getCellType(inputCellIndex))){
        for(i=0; i<size; i++) {
            ret[i] = behaviour->getOutputStrength(tissue_getCellType(inputCellIndex), inputStrength, ret[i]);
        }
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

            behaviour->processIncomingConnections(tissue_getCellType(cellIndex), count, cellIndex, indexesOfIncomingConnections, strengths);

            free(indexesOfIncomingConnections);
            free(strengths);
        } else {
            #ifndef NDEBUG
            printf("Cell at %d has no incoming connections -- no action required\n", cellIndex);
            #endif
        }
    } else {
        #ifdef TRACEDEBUG
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

            behaviour->processOutgoingConnections(tissue_getCellType(cellIndex), count, cellIndex, indexesOfOutgoingConnections, strengths);

            free(indexesOfOutgoingConnections);
            free(strengths);

        } else{
            #ifndef NDEBUG
            printf("Cell at %d has no outgoing connections -- no action required\n", cellIndex);
            #endif
        }
    } else {
        #ifdef TRACEDEBUG
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

static void doMatrixFeedforwardStim(int * targets, double * strengths, int count, BitArray *touchedCellIndexes) {
    int i;

    for(i=0; i<count; i++) {
        bitarray_writeBit(touchedCellIndexes, targets[i], on);
    }

    //  Step 1:  Determine all cells to which at least one of the target cells is connected
    int totalOutputCount = 0;

    BitArray *presentConnections = bitarray_create(NUM_CELLS);

    for(i=0; i<count; i++) {
        int connectionCount = cells_countConnectedFrom(targets[i]);
        int * endpointIndexes = cells_indexesOfConnectedFrom(targets[i]);

        int j; 
        for(j=0; j<connectionCount; j++) {
            bitarray_writeBit(presentConnections, endpointIndexes[j], on);
        }

        free(endpointIndexes);

    }

    int numEndpoints = bitarray_countOn(presentConnections, NUM_CELLS);

    if(numEndpoints == 0) {

        #ifndef NDEBUG
        printf("Reached end of recursive feedforward stim\n");
        #endif

        for(i=0; i<count; i++){

            #ifndef NDEBUG
            printf("(%d)  (feedfwd final) %d -> %f\n", i, targets[i], strengths[i]);
            #endif

            tissue_state_updateOutputToCell(targets[i], strengths[i]);
        }

        bitarray_destroy(presentConnections);

        return;

    }

    int * endpointIndexes = bitarray_indexesTurnedOn(presentConnections, NUM_CELLS);

    //  Step 2:  Create weight matrix
    int outputCellIndex;
    int inputCellIndex;

    double weightMatrix[numEndpoints][count];

    for(inputCellIndex=0; inputCellIndex<count; inputCellIndex++) {

        for(outputCellIndex=0; outputCellIndex<numEndpoints; outputCellIndex++) {

            weightMatrix[outputCellIndex][inputCellIndex] = cells_strengthOfConnection(targets[inputCellIndex], endpointIndexes[outputCellIndex]);

        }

    }

    //  Step 3:  Multiply input vector by weight matrix
    double outputs[numEndpoints];
    for(outputCellIndex=0; outputCellIndex<numEndpoints; outputCellIndex++) {

        double result = 0.0;

        for(inputCellIndex=0; inputCellIndex<count; inputCellIndex++) {

            double inputValue = strengths[inputCellIndex];
            result += inputValue * weightMatrix[outputCellIndex][inputCellIndex];

        }

        outputs[outputCellIndex] = result;
    }

    //  Step 4:  Calculate Neuronal activations
    for(outputCellIndex=0; outputCellIndex<numEndpoints; outputCellIndex++) {
        int cellType = tissue_getCellType(endpointIndexes[outputCellIndex]);
        if(cellTypes_existsActivationFunction(cellType)) {
            outputs[outputCellIndex] = cellTypes_behaviourFor(cellType)->calculateActivation(outputs[outputCellIndex]);
        }
    }

    //  Step 5:  Update network state

    #ifndef NDEBUG

    printf("Feedforward stim result\n------------\n");
    int k;
    for(k=0; k<numEndpoints; k++) {
        printf("%d -> [\t%f\t]\n", endpointIndexes[k], outputs[k]);
    }
    printf("END FEEDFORWARD RES\n");

    #endif

    //  Step 6:  Continue to stimulate next layer
    doMatrixFeedforwardStim(endpointIndexes, outputs, numEndpoints, touchedCellIndexes);

    //  Step 7:  Memory cleanup
    bitarray_destroy(presentConnections);
    free(endpointIndexes);

    
}

void cells_stimulate(int * targets, double * strengths, int count) {

    //  Reset state before doing any computation
    _getState(1);
    
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

void cells_matrix_feedforward_stim(int * targets, double * strengths, int count) {

    //  Reset state before doing any computation
    _getState(1);

    BitArray *touchedCellIndexes = bitarray_create(NUM_CELLS);

    doMatrixFeedforwardStim(targets, strengths, count, touchedCellIndexes);

    int i;
    for(i=0; i<NUM_CELLS; i++) {
        if(bitarray_valueOf(touchedCellIndexes, i) == on) {
            doCellBehaviourAndLogic(i);
        }
    }

    bitarray_destroy(touchedCellIndexes);

    tissue_pushState();

}



#endif