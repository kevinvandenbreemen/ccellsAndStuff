# ccellsAndStuff
Simple C programming project to play with arrays

# Table of Contents
<!-- TOC -->

- [ccellsAndStuff](#ccellsandstuff)
- [Table of Contents](#table-of-contents)
- [Setup](#setup)
  - [Building](#building)
  - [Unit Tests](#unit-tests)
    - [Further help on "check" library](#further-help-on-%22check%22-library)
  - [Regression](#regression)
- [Getting Started](#getting-started)
  - [Setting up the Tissue](#setting-up-the-tissue)
  - [Connecting Two Cells with Strength](#connecting-two-cells-with-strength)
  - [Sending a Pulse Through the Network](#sending-a-pulse-through-the-network)
  - [Feedforward Stimulation](#feedforward-stimulation)
    - [Output Activations](#output-activations)
      - [Built-in Activation Functions](#built-in-activation-functions)
  - [Getting Output from the Network](#getting-output-from-the-network)
    - [Output Callbacks](#output-callbacks)
  - [Tying it All Together](#tying-it-all-together)
  - [Some Approaches to Learning](#some-approaches-to-learning)
- [Data Structure](#data-structure)
  - [Over-arching](#over-arching)
- [Swift API](#swift-api)
  - [Getting Started](#getting-started-1)
    - [Interacting with the Tissue](#interacting-with-the-tissue)
    - [Connecting Cells](#connecting-cells)
    - [Cell Types](#cell-types)
      - [The CellType API](#the-celltype-api)
    - [Stimulating the Network](#stimulating-the-network)
      - [Tissue State Callback](#tissue-state-callback)
  - [Architecture](#architecture)

<!-- /TOC -->

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

### Output Activations
By default neurons in the network will simply send whatever the sum total of their stimulation is.  This is intentional, as it allows for greater flexibility in how you choose to calculate outputs.

You can have cells of a specific type calculate activation / output by calling

```
cellTypes_setActivationCalculation(int cellType, (*calculateActivation)(int cellType, double weightedInputSum))
```

#### Built-in Activation Functions
You can find built-in functions in **celltypeFunctions.h**.  The following functions are provided.

*activationCalculation_sigmoid* - Sigmoid Activation function using 1 / (1 + e^-x)

## Getting Output from the Network

```
    TissueState * state = tissue_getState();
    fail_unless(state->outputCount == 3, "3 outputs expected");

    int * destIndexes = state->outputIndices;
    double * destStrengths = state->outputStrengths;
```

The state.outputIndices indicates the final cells that received output from your input.  The output strengths are the actual strengths of output at each index

**Important Note**:  You will want to get the state before your next call to either cells_stimulate() or cells_matrix_feedforward_stim(), as these methods will reset the tissue state before they execute.

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

## Getting Started
I have built a layer of Swift on top of the projet in order to allow for a supply of Swift Linux coding training opportunities.



### Interacting with the Tissue
You can use the TissueManager to set up cell connections, re-initialize, etc, the tissue.

```
        let tissueManager = TissueManager()
```

### Connecting Cells
You can connect cells using the TissueManager.connectCell() method

```
        tissueManager.connectCell(from: 0, to: 3, strength: 0.5)
        tissueManager.connectCell(from: 1, to: 4, strength: 0.5)
```

### Cell Types
Every cell in the tissue has a type.  You can determine this by interrogating individual cells in the tissue.

```
        guard let cell = tissueManager.cell(at: 0) else {
            return
        }

        let cellType = cell.type
```

#### The CellType API
Each cell's type has an API that lets you configure its behaviour.  This allows for a great deal of flexibility in how cells in the network will behave, as you can supply your own application logic for how cells connect to one another.

You can use method CellType.setLogic(to logic: CellTypeLogic) to accomplish this.

For more details please see also the **CellTypeLogic** class.

### Stimulating the Network

You can send data into the network using the feedforwardStimulate() method.  This method is so-named because the internal algorithm simply determines the cells that the "target" cells you choose to stimulate are connected from.  It then recursively stimulates those cells with the outputs of your first stimulations etc. until a set of terminal cells are arrived at.

```
        var targets: [Int32] = [0, 1, 2]

        var strengths = [0.2, 0.9, 1.77722]

        var callbackReceived = false
        tissueManager.setTissueStateCallback { state in 
            callbackReceived = true

            print("Callback received:  Data:  \(state.outputIndexes), \(state.outputStrengths)")

        }

        tissueManager.feedforwardStimulate(cellIndexes: &targets[0], strengths: &strengths[0], count: Int32(targets.count))

```

#### Tissue State Callback
In example above you'll notice that we register a callback.  This callback lets you receive output from the network and react to it.

## Architecture

![Swift API](docs/res/cCellsAndStuff-Swift&#32;Integration.svg)