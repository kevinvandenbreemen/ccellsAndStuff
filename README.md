# ccellsAndStuff
Simple C programming project to play with arrays

# Setup

## Building
Simply use the makefile.  Run run the program type make run

## Unit Tests
I developed this on an Ubuntu machine.  I tried to add a check to the Makefile for the requisite "Check" library.  If it fails you can run 

```
sudo apt-get install check
```

on your Ubuntu box.

### Further help on "check" library

I followed https://stackoverflow.com/a/54095032/2328196 to get myself up and running.

## Regression

You can use the 'pipeline' make target to run both unit tests and memory leak check.

# Getting Started

## Setting up the Tissue

Before you can use the library you must initialize a tissue.  The library provides a default tissue initializer here:

```
    tissue_initializeDefaultTissue();
```

## Connecting Two Cells with Strength

```
    cells_connectDirected(10, 11, 0.5);
```

## Sending a Pulse Through the Network
```
    int targets[] = {10, 11};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_stimulate(targets, strengths, count);
```

## Feedforward Stimulation
You can use the tissue like a feedforward neural network by calling 

```
cells_matrix_feedforward_stim(int * targets, double * strengths, int count)
```

This function will take the target cells you picked, determine all of the endpoints of those cells, and build an adjacency matrix (weight matrix), sending your data.  If the endpoints themselves have connections the function will then propagate the signal forward to those endpoints as well.

## Getting Output from the Network

```
    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 3, "3 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;
```

The state.outputIndices indicates the final cells that received output from your input.  The output strengths are the actual strengths of output at each index

### Output Callbacks
If you wish to program using callbacks instead of explicitly requesting the states, you can call 

```
tissue_setOnStateUpdate(void (*onStateUpdate)(TissueState * state) )
```

## Tying it All Together

```
    //  Initialize the tissue
    tissue_initializeDefaultTissue();

    //  Connect the cells together
    cells_connectDirected(10, 11, 0.5);
    cells_connectDirected(10, 12, 0.1);
    cells_connectDirected(11, 13, 1.0);

    //  Stimulate the Cells
    int targets[] = {10, 11};
    double strengths[] = {1.0, 1.0};
    int count = 2;
    cells_stimulate(targets, strengths, count);

    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 3, "3 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;
```

## Some Approaches to Learning
You can add logic for individual cells to perform as the network executes.  

You can do this for both incoming connections and outgoing connections.  Both of these require similar function pointers.

For example, for outgoing connections, provide a pointer to a function that takes the following arguments:
```
(int cellType, int size, int cellIndex, int * outgoingIndexes, double * outgoingStrengths)
```

to method 

**cellTypes_setCellLogicForOutgoingConnections**

Similarly, for incoming connections, provide a pointer to a function that takes the following arguments:
```
(int cellType, int size, int cellIndex, int * incomingIndexes, double * incomingStrengths)
```

to method

**cellTypes_setCellLogicForIncomingConnections**

# Data Structure

## Over-arching

![Graph Structure](./docs/res/cCellsAndStuff.svg)

# Swift API

## Architecture

![Swift API](docs/res/cCellsAndStuff-Swift&#32;Integration.svg)