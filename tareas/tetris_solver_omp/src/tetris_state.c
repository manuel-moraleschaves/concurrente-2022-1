// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"

void destroy_tetris(tetris_t* tetris) {
    free_matrix(tetris->rows, (void**)tetris->matrix);
    destroy_levels(tetris->levels, tetris->rows);
    free(tetris->figure_sequence);
    free(tetris);
}

tetris_t* clone_tetris(tetris_t* base_tetris) {
    tetris_t* tetris = malloc(sizeof(tetris_t));

    if (!tetris) {
        return NULL;
    }

    tetris->id = base_tetris->id;
    tetris->depth = base_tetris->depth;
    tetris->rows = base_tetris->rows;
    tetris->columns = base_tetris->columns;

    tetris->matrix = clone_matrix(base_tetris->matrix, tetris->rows,
                                  tetris->columns);

    if (!tetris->matrix) {
        fprintf(stderr, "Error: could not create the matrix.\n");
        free(tetris);
        return NULL;
    }

    tetris->sequence_count = base_tetris->sequence_count;

    tetris->figure_sequence = (char*) calloc(tetris->sequence_count + 1,
                                sizeof(char));

    if (!tetris->figure_sequence) {
        fprintf(stderr, "Error: could not create the figure sequence.\n");
        free(tetris);
        return NULL;
    }

    snprintf(tetris->figure_sequence, tetris->sequence_count + 1,
                "%s", base_tetris->figure_sequence);

    #pragma omp critical(height)
    {
        tetris->min_height = base_tetris->min_height;
    }

    tetris->levels = create_level('B', 0, tetris->rows, tetris->columns,
                                  tetris->matrix);
    if (!tetris->levels) {
        fprintf(stderr, "Error: could not create the base level.\n");
        free(tetris);
        return NULL;
    }

    return tetris;
}
