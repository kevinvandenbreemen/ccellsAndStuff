#ifndef IN_DISPLAY
#define IN_DISPLAY

#include <SDL2/SDL.h>
#include <math.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Display the current tissue
 */
void tissue_display_tissue();

/**
 * Tears down display, freeing up any resources it was using
 */
void tissue_close_display();

static BitArray * getDisplaySettings() {

    static int initialized = 0;

    static BitArray * ret;
    
    if(initialized == 0) {
        ret = bitarray_create(2);
        initialized = 1;
    }

    return ret;
}

static void doSDLSetup() {
    
    if(bitarray_valueOf(getDisplaySettings(), 0) == 0) {
        printf("Creating SDL......\n");
        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            fprintf(stderr, "Could not initialize SDL\n");
            return;
        }

        printf("Initialized SDL!\n");
        bitarray_writeBit(getDisplaySettings(), 0, 1);
    }
}

static SDL_Window * getWindow() {


    static SDL_Window *window;

    if(bitarray_valueOf(getDisplaySettings(), 1) != 1) {
        printf("Creating Window...\n");
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("Could not initialize SDL system!\n");
            return NULL;
        }
        window = SDL_CreateWindow("Tissue Cells Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        bitarray_writeBit(getDisplaySettings(), 1, 1);
    }

    return window;
}

static SDL_Renderer * getRenderer() {

    static SDL_Renderer *renderer;

    if(bitarray_valueOf(getDisplaySettings(), 2) != 1) {
        printf("Creating Renderer...\n");
        SDL_Window *window = getWindow();
        if(window == NULL) {
            return NULL;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        bitarray_writeBit(getDisplaySettings(), 2, 1);
    }

    return renderer;
}

static void drawCells(SDL_Renderer *renderer) {

    int numCells = tissue_getNumCells();
    int root = (int) ceil(sqrt((double)numCells));
    int area = SCREEN_WIDTH * SCREEN_HEIGHT;
    int cellHeight = (int)floor(SCREEN_HEIGHT / root);
    int cellWidth = (int)floor(SCREEN_WIDTH / root);

    #ifndef NDEBUG
    printf("Render Cells: root=%d, area=%d, cellSize[w,h]=(%d, %d)\n", root, area, cellWidth, cellHeight);
    #endif

    TissueState *state = tissue_getState();
    BitArray *activeCells = bitarray_create(numCells);
    

    int i, j, index;

    for(i=0; i<state->outputCount; i++) {
        bitarray_writeBit(activeCells, state->outputIndices[i], on);
    }

    SDL_Rect rect;
    index = 0;
    for(i=0; i<root; i++) {
        for(j=0; j<root; j++) {

            if(index >= numCells) {
                break;
            }

            rect.x = (j * cellWidth);
            rect.y = (i * cellHeight);
            rect.h = cellHeight - 5;
            rect.w = cellWidth - 5;

            #ifdef DEBUG
            printf("render rect = {x=%d, y=%d, h=%d, w=%d}\n", rect.x, rect.y, rect.h, rect.w);
            #endif

            if(bitarray_valueOf(activeCells, index) == on){
                SDL_SetRenderDrawColor(renderer, 255, 233, 233, 100);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            }

            SDL_RenderFillRect(renderer, &rect);

            index ++;
        }
    }

    bitarray_destroy(activeCells);
    free(state->outputIndices);
    free(state->outputStrengths);
    free(state);
}

void tissue_close_display() {
    SDL_DestroyRenderer(getRenderer());
    SDL_DestroyWindow(getWindow());
    SDL_Quit();

    bitarray_writeBit(getDisplaySettings(), 0, 0);
    bitarray_writeBit(getDisplaySettings(), 1, 0);
}

void tissue_display_tissue() {

    doSDLSetup();
    SDL_Renderer *renderer = getRenderer();
    if(renderer == NULL) {
        return;
    }

    //	Clear the screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //	Draw on screen
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    SDL_RenderDrawPoint(renderer, 100, 100);

    drawCells(renderer);

    //	Show what was drawn
    SDL_RenderPresent(renderer);
}

#endif