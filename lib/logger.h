#ifndef LOGGER_IN
#define LOGGER_IN

#include <stdio.h>

void logD(char * message) {

    #ifndef NDEBUG
    printf("DEBUG\t%s\n", message);
    #endif

}

void logI(char * message) {
    printf("INFO\t%s\n", message);
}

#endif