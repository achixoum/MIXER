cmake_minimum_required(VERSION 3.30)
project(Compiler C)

set(CMAKE_C_STANDARD 11)

add_executable(Compiler
        syndax_tree.c
        symbol_table.c
        mixal_generation.c
        mixal_generation.h
        log.c
)
