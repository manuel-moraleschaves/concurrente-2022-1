// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"

void destroy_tetris(tetris_t* tetris) {
    free_matrix(tetris->rows, (void**)tetris->matrix);
    destroy_levels(tetris->levels, tetris->rows);
    free(tetris->figure_sequence);
    free(tetris);
}

tetris_t* clone_tetris(shared_data_t* shared_data) {
    tetris_t* source = shared_data->tetris;
    tetris_t* tetris = malloc(sizeof(tetris_t));

    if (!tetris) {
        return NULL;
    }

    tetris->id = source->id;
    tetris->depth = source->depth;
    tetris->rows = source->rows;
    tetris->columns = source->columns;

    tetris->matrix = clone_matrix(source->matrix, tetris->rows,
                                  tetris->columns);

    if (!tetris->matrix) {
        fprintf(stderr, "Error: could not create the matrix.\n");
        free(tetris);
        return NULL;
    }

    tetris->sequence_count = source->sequence_count;

    tetris->figure_sequence = (char*) calloc(tetris->sequence_count + 1,
                                sizeof(char));

    if (!tetris->figure_sequence) {
        fprintf(stderr, "Error: could not create the figure sequence.\n");
        free(tetris);
        return NULL;
    }

    snprintf(tetris->figure_sequence, tetris->sequence_count + 1,
                "%s", source->figure_sequence);

    pthread_mutex_lock(&shared_data->can_access_min_height);
    tetris->min_height = source->min_height;
    pthread_mutex_unlock(&shared_data->can_access_min_height);

    tetris->levels = create_level('B', 0, tetris->rows, tetris->columns,
                                  tetris->matrix);
    if (!tetris->levels) {
        fprintf(stderr, "Error: could not create the base level.\n");
        free(tetris);
        return NULL;
    }

    return tetris;
}
