// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_STATE_H
#define TETRIS_STATE_H

#include <stdio.h>
#include <stdlib.h>

#include "tetris_level.h"

typedef struct {
    size_t id;
    int depth;
    int rows;
    int columns;
    char** matrix;
    int sequence_count;
    char* figure_sequence;
    int min_height;
    level_t* levels;
} tetris_t;

tetris_t* read_tetris(FILE* file);
void destroy_tetris(tetris_t* tetris);
int solve_tetris_dfs(tetris_t* tetris, int piece_index);

#endif
