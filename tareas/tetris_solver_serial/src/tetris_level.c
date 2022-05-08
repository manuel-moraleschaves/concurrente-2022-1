// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_level.h"
#include "tetris_utils.h"

level_t* save_level(char figure, int shape, int num_rows,
        int num_cols, char** matrix) {
    level_t* level = malloc(sizeof(level_t));

    level->figure = figure;
    level->shape = shape;
    level->matrix = (char**)create_matrix(num_rows,
                        num_cols + 1, sizeof(char));

    if (!level->matrix) {
        fprintf(stderr, "Error: could not create the level record.\n");
        free(level);
        return NULL;
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            level->matrix[i][j] = matrix[i][j];
        }
    }

    return level;
}
