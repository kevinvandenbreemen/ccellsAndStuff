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

    tissue_display_tissue();
    SDL_Delay(5000);
    tissue_display_tissue();
    SDL_Delay(5000);
    tissue_close_display();
}