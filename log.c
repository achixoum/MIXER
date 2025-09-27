#include "log.h"

#include <string.h>

FILE* file = NULL;   // define the global variable (allocated here)
FILE* mixal_file = NULL;

void init_file(const char* file_path) {
    if (strcmp(file_path, "mixal.mixal") == 0)
        mixal_file = fopen(file_path, "w");
    else {
        file = fopen(file_path, "w");
        fprintf(file, "-------------FLEX AND YACC DEBUGGING LOGS-------------\n\n");
    }
}

void close_files() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
    if (mixal_file != NULL) {
        fclose(mixal_file);
        mixal_file = NULL;
    }
}