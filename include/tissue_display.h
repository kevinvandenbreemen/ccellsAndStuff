#ifndef IN_DISPLAY
#define IN_DISPLAY

#include <SDL2/SDL.h>

/**
 * Display the current tissue
 */
void tissue_display_tissue();

/**
 * Tears down display, freeing up any resources it was using
 */
void tissue_close_display();

static BitArray * getDisplaySettings() {
    static BitArray * ret;
    ret = bitarray_create(2);
    return ret;
}

static void doSDLSetup() {
    
    if(bitarray_valueOf(getDisplaySettings(), 0) == 0) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0){
            fprintf(stderr, "Could not initialize SDL\n");
            return;
        }

        printf("Initialized SDL!\n");
        bitarray_writeBit(getDisplaySettings(), 0, 1);
    }
}

static SDL_Window * getWindow() {
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    static SDL_Window *window;

    if(bitarray_valueOf(getDisplaySettings(), 1) != 1) {
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

    int rendererInitialized = 0;
    static SDL_Renderer *renderer;

    if(rendererInitialized != 1) {
        SDL_Window *window = getWindow();
        if(window == NULL) {
            return NULL;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        rendererInitialized = 1;
    }

    return renderer;
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

    //	Show what was drawn
    SDL_RenderPresent(renderer);
}

#endif