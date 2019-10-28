#ifndef IN_CELL_TYPE_FUNCTIONS
#define IN_CELL_TYPE_FUNCTIONS

//  Basic cell type
double basic_calculateOutputStrength(int cellType, double inputStrength, double rawOutputStrength) {
    return rawOutputStrength;
}

//  Inhibitory cell type
double inhibitory_calculateOutputStrength(int cellType, double inputStrength, double rawOutputStrength) {
    return -1 * (inputStrength * rawOutputStrength);
}

double activationCalculation_sigmoid(double weightedSum) {
    double exp_value = exp((double) -weightedSum);
    return 1 / (1 + exp_value);
}

#endif