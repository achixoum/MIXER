#ifndef LOG_H
#define LOG_H

#include <stdio.h>

extern FILE* file;       //the file to output the logs, syntax tree and symbol table
extern FILE* mixal_file; //the file for the assembly MIXAL output

void init_file(const char* file_path);
void close_files();

#endif //LOG_H
