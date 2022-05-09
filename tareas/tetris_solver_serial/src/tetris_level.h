// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_LEVEL_H
#define TETRIS_LEVEL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char figure;
    int shape;
    char** matrix;
} level_t;

int save_level(level_t* level, char figure, int shape, int num_rows,
        int num_cols, char** matrix);

#endif
