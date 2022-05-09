// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_UTILS_H
#define TETRIS_UTILS_H

#include <stdlib.h>

void** create_matrix(size_t row_count,
                     size_t col_count, size_t element_size);

void free_matrix(const size_t row_count, void** matrix);

void print_matrix(const size_t row_count, char** matrix);
void print_matrix_file(const size_t row_count, char** matrix, FILE* file);

#endif
