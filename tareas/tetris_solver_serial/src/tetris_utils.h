// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_UTILS_H
#define TETRIS_UTILS_H

#include <stdlib.h>

/**
 * @brief Rutina para crear una matriz.
 * @details Crea una matriz con la cantidad de filas y columnas indicadas.
 * @param row_count Cantidad de filas de la matriz.
 * @param col_count Cantidad de columnas de la matriz.
 * @param element_size Tamaño del tipo de elemento de la matriz.
 * @return La matrix creada en forma de void**.
 */
void** create_matrix(size_t row_count,
                     size_t col_count, size_t element_size);

/**
 * @brief Rutina para liberar una matriz.
 * @details Elimina y libera los datos en memoria de una matriz.
 * @param row_count Cantidad de filas de la matriz.
 * @param matrix Matriz que se desea eliminar.
 * @return void.
 */
void free_matrix(const size_t row_count, void** matrix);

/**
 * @brief Rutina para imprimir una matriz.
 * @details Imprime en la salida estándar los datos de la matriz.
 * @param row_count Cantidad de filas de la matriz.
 * @param matrix Matriz que se desea imprimir.
 * @return void.
 */
void print_matrix(const size_t row_count, char** matrix);

/**
 * @brief Rutina para imprimir una matriz en archivo.
 * @details Imprime en el archivo especificado los datos de la matriz.
 * @param row_count Cantidad de filas de la matriz.
 * @param matrix Matriz que se desea imprimir.
 * @param file Puntero al archivo de salida.
 * @return void.
 */
void print_matrix_file(const size_t row_count, char** matrix, FILE* file);

/**
 * @brief Rutina para clonar una matriz.
 * @details Crear una nueva matriz y clona los datos de la amtriz origen.
 * @param matrix Matriz que se desea clonar.
 * @param row_count Cantidad de filas de la matriz.
 * @param col_count Cantidad de columnas de la matriz.
 * @return La nueva matriz char** clonada.
 */
char** clone_matrix(char** matrix, size_t row_count, size_t col_count);

#endif
