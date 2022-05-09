// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_level.h"
#include "tetris_utils.h"

int save_level(level_t* level, char figure, int shape, int num_rows,
        int num_cols, char** matrix) {
    if (level->matrix) {
        free_matrix(num_rows, (void**)level->matrix);
    }

    level->figure = figure;
    level->shape = shape;
    level->matrix = (char**)create_matrix(num_rows,
                        num_cols + 1, sizeof(char));

    if (!level->matrix) {
        fprintf(stderr, "Error: could not create the level record.\n");
        free(level);
        return 0;
    }
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            level->matrix[i][j] = matrix[i][j];
        }
    }

    return 1;
}
