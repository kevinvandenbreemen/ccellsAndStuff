#ifndef IN_SWFT_INTEGRATION
#define IN_SWFT_INTEGRATION

#include <stdlib.h>
#include <stdio.h>

#include "../../../include/logger.h"
#include "../../../include/bitarray.h"
#include "../../../include/cellTypeFunctions.h"
#include "../../../include/cellTypes.h"
#include "../../../include/tissue.h"
#include "../../../include/connections.h"
#include "../../../include/math.h"
#include "../../../include/msgs.h"

#define MAJ_VER 1
#define MIN_VER 0
#define REV_VER 0

int majorVersion();
int minorVersion();
int revisionNum();

void swift_tissue_initializeDefaultTissue();

int swift_cells_connectDirected(int from, int to, double strength);

//  Cell Types
int swift_tissue_getCellType(int index);

/**
 * Configures the behaviour for a given cell type to the given CellTypeBehaviour
 */
void swift_cellTypes_setBehaviourForCellType(int cellType, CellTypeBehaviour *behaviour);

//  Network Stimulation
void swift_cells_stimulate(int * targets, double * strengths, int count);
void swift_cells_matrix_feedfoward_stim(int * targets, double * strengths, int count);

#endif