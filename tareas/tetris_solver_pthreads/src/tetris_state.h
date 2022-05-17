// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_STATE_H
#define TETRIS_STATE_H

#include <stdio.h>
#include <stdlib.h>

#include "tetris_level.h"

/**
 * @brief Estructura para manejo del estado del tetris.
 * @details Contiene el id del juego, la profundidad, la cantidad de columnas, la matriz de posiciones,
 * la catidad de figuras que están cayendo, la secuencia de esas figuras y el nodo base de los niveles.
 */
typedef struct {
    size_t id;
    int depth;
    int rows;
    int columns;
    char** matrix;
    int sequence_count;
    char* figure_sequence;
    int min_height;
    struct level_t* levels;
} tetris_t;

/**
 * @brief Rutina para leer el estado inicial del tetris.
 * @details Lee la información del archivo de entrada y crea el estado inicial del tetris.
 * @param file Puntero al archivo de entrada.
 * @return Un puntero al estado inicial creado o NULL en caso de error.
 */
tetris_t* read_tetris(FILE* file);

/**
 * @brief Rutina destruir el estado del tetris.
 * @details Elimina y libera los datos almacenados en memoria del estado del tetris.
 * @param tetris Puntero al estado del tetris.
 * @return Void.
 */
void destroy_tetris(tetris_t* tetris);

/**
 * @brief Rutina recursiva y paralela para la solución del tetris.
 * @details Rutina recursiva que mediante hilos y un algoritmo de Búsqueda en Profundidad soluciona el tetris.
 * @param tetris Puntero al estado del tetris.
 * @param piece_index Índice de la figura que está cayendo.
 * @param base_level Puntero al nodo base de los niveles.
 * @return Un valor entero: 0 en caso de algún, 1 en caso de éxito.
 */
int solve_tetris_dfs(tetris_t* tetris, int piece_index,
                     struct level_t* base_level);

/**
 * @brief Rutina para iniciar la solución del tetris.
 * @details Recorre el nivel 0 y llama a la rutina recursiva dfs.
 * @param tetris Puntero al estado del tetris.
 * @param base_level Puntero al nodo base de los niveles.
 * @return Un valor entero: 0 en caso de algún, 1 en caso de éxito.
 */
int solve_tetris(tetris_t* tetris, struct level_t* base_level);

#endif
