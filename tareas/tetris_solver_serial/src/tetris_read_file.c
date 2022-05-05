// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_read_file.h"

void** create_matrix(size_t row_count,
                     size_t col_count, size_t element_size);
void free_matrix(const size_t row_count, void** matrix);
void print_matrix(tetris_t* tetris);


tetris_t* read_tetris(FILE* file) {
    tetris_t* tetris = malloc(sizeof(tetris_t));

    if (fscanf(file, "%zu", &tetris->id) == 0 || tetris->id <= 0) {
        fprintf(stderr, "Error: invalid identifier in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("ID: %zu\n", tetris->id);

    if (fscanf(file, "%i", &tetris->depth) == 0 || tetris->depth <= 0) {
        fprintf(stderr, "Error: invalid depth in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Depth: %d\n", tetris->depth);

    if (fscanf(file, "%i", &tetris->rows) == 0 || tetris->rows <= 0) {
        fprintf(stderr, "Error: invalid rows in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Rows: %d\n", tetris->rows);

    if (fscanf(file, "%i", &tetris->columns) == 0 || tetris->columns <= 0) {
        fprintf(stderr, "Error: invalid columns in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Columns: %d\n", tetris->columns);

    tetris->matrix = (char**)create_matrix(tetris->rows,
                        tetris->columns + 1, sizeof(char));

    if (!tetris->matrix) {
        fprintf(stderr, "Error: could not create the tetris matrix.\n");
        free(tetris);
        return NULL;
    }

    for (int i = 0; i < tetris->rows; ++i) {
        fscanf(file, "%s", tetris->matrix[i]);
    }

    print_matrix(tetris);

    if (fscanf(file, "%i", &tetris->sequence_count) == 0
        || tetris->sequence_count <= 0
        || tetris->sequence_count < tetris->depth+1) {
        fprintf(stderr, "Error: invalid sequence count in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("K: %d\n", tetris->sequence_count);

    tetris->figure_sequence = (char*) calloc(tetris->sequence_count + 1,
                                sizeof(char));

    if (!tetris->figure_sequence) {
        fprintf(stderr, "Error: could not create the figure sequence.\n");
        free(tetris);
        return NULL;
    }

    for (int i = 0; i < tetris->sequence_count; ++i) {
        getc(file);
        tetris->figure_sequence[i] = getc(file);
    }
    tetris->figure_sequence[tetris->sequence_count] = '\0';
    printf("Figure sequence: %s\n", tetris->figure_sequence);

    return tetris;
}

void destroy_tetris(tetris_t* tetris) {
    free_matrix(tetris->rows, (void**)tetris->matrix);
    free(tetris->figure_sequence);
    free(tetris);
}

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

void print_matrix(tetris_t* tetris) {
    for (int i = 0; i < tetris->rows; ++i) {
        printf("%s\n", tetris->matrix[i]);
    }
}
