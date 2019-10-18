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

## Getting Output from the Network

```
    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 3, "3 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;
```

The state.outputIndices indicates the final cells that received output from your input.  The output strengths are the actual strengths of output at each index

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

# Data Structure

## Over-arching

![Graph Structure](./docs/res/cCellsAndStuff.svg)