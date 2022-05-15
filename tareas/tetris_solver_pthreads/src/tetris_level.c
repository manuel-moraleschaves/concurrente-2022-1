// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_level.h"
#include "tetris_utils.h"


struct level_t* create_level(char figure, int rotation, int num_rows,
                             int num_cols, char** matrix) {
    struct level_t* new_level = malloc(sizeof(struct level_t));

    if (!new_level) {
        return NULL;
    }

    new_level->figure = figure;
    new_level->rotation = rotation;
    new_level->next = NULL;

    new_level->matrix = clone_matrix(matrix, num_rows, num_cols);

    if (!new_level->matrix) {
        free(new_level);
        return NULL;
    }

    return new_level;
}

void clone_level(struct level_t* source, struct level_t* dest,
                 int num_rows, int num_cols) {
    struct level_t* current = dest;

    // Destruye los niveles siguientes que tenga el destino
    if (dest->next) {
        destroy_levels(dest->next, num_rows);
    }

    current = source->next;
    while (current != NULL) {
        dest->next = create_level(current->figure, current->rotation, num_rows,
                                  num_cols, current->matrix);
        current = current->next;
        dest = dest->next;
    }
}

void destroy_levels(struct level_t* source , int num_rows) {
    if (source->next) {
        struct level_t * temp = source->next;
        struct level_t * next;
        do {
            next = temp->next;
            free_matrix(num_rows, (void**) temp->matrix);
            free(temp);
            temp = next;
        } while (temp);
    }
    free_matrix(num_rows, (void**)source->matrix);
    free(source);
}
