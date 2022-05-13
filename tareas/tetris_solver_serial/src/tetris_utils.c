// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include <stdio.h>

#include "tetris_utils.h"

void** create_matrix(size_t row_count,
                     size_t col_count, size_t element_size) {
    void** matrix = (void**) calloc(row_count, sizeof(void*));
    if ( matrix == NULL ) {
        return NULL;
    }

    for (size_t row = 0; row < row_count; ++row) {
        if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
            free_matrix(row_count, matrix);
            return NULL;
        }
    }

    return matrix;
}

void free_matrix(const size_t row_count, void** matrix) {
    if (matrix) {
        for (size_t row = 0; row < row_count; ++row) {
            free(matrix[row]);
        }
    }

    free(matrix);
}

void print_matrix(const size_t row_count, char** matrix) {
    if (matrix) {
        for (size_t row = 0; row < row_count; ++row) {
            printf("%s\n", (char*) matrix[row]);
        }
    }
}

void print_matrix_file(const size_t row_count, char** matrix, FILE* file) {
    if (matrix) {
        for (size_t row = 0; row < row_count; ++row) {
            fprintf(file, "%s\n", (char*) matrix[row]);
        }
    }
}

char** clone_matrix(char** matrix, size_t row_count, size_t col_count) {
    char** new_matrix = (char**)create_matrix(row_count,
                                    col_count + 1, sizeof(char));

    if (!new_matrix || !matrix) {
        return NULL;
    }

    for (size_t i = 0; i < row_count; ++i) {
        for (size_t j = 0; j < col_count; ++j) {
            new_matrix[i][j] = matrix[i][j];
        }
    }

    return new_matrix;
}
