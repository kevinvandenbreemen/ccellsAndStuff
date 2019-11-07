#include <stdio.h>

//  Include all code for this system
#include "include/logger.h"
#include "include/bitarray.h"
#include "include/cellTypeFunctions.h"
#include "include/cellTypes.h"
#include "include/tissue.h"
#include "include/connections.h"
#include "include/math.h"
#include "include/msgs.h"
#include "include/driver.h"
#include "include/tissue_display.h"

int main() {
    printf("This is a project for learning about and using array functions to create relating cell things\n");
    tissue_initializeDefaultTissue();

    cells_connectDirected(0, 10, 0.24);
    cells_connectDirected(1, 10, 2.2);
    cells_connectDirected(2, 10, -0.4);
    cells_connectDirected(3, 10, 4.9);
    cells_connectDirected(2, 3, 4.9);

    int targets[] = {0,1,2,3};
    double strengths[] = {1.0, 1.0, 10.3, 1.2};
    int count = 4;
    
    cells_stimulate(targets, strengths, count);
    cells_matrix_feedforward_stim(targets, strengths, count);

    tissue_display_tissue();
    SDL_Delay(5000);
    tissue_display_tissue();
    SDL_Delay(5000);
    tissue_close_display();
}