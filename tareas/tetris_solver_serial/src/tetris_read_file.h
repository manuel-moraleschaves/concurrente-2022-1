// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_READ_FILE_H
#define TETRIS_READ_FILE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t id;
    int depth;
    int rows;
    int columns;
    char** matrix;
    int sequence_count;
    char* figure_sequence;
} tetris_t;

tetris_t* read_tetris(FILE* file);
void destroy_tetris(tetris_t* tetris);

#endif
