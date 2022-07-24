// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_SOLVE_H
#define TETRIS_SOLVE_H

#include <stdio.h>
#include <stdlib.h>

#include "tetris_level.h"
#include "tetris_state.h"

/**
 * @brief Rutina recursiva y paralela para la solución del tetris.
 * @details Rutina recursiva que mediante hilos y un algoritmo de Búsqueda en 
 * Profundidad soluciona el tetris.
 * @param tetris Puntero al estado del tetris.
 * @param piece_index Índice de la figura que está cayendo.
 * @param base_level Puntero al nodo base de los niveles.
 * @param base_tetris Puntero al estado inicial del tetris.
 * @return Un valor entero: 0 en caso de algún, 1 en caso de éxito.
 */
int solve_tetris_dfs(tetris_t* tetris, int piece_index,
                     struct level_t* base_level, tetris_t* base_tetris);

/**
 * @brief Rutina para iniciar la solución del tetris en nivel 1.
 * @details Recorre el nivel 1 y llama a la rutina recursiva dfs.
 * @param base_tetris Puntero al estado inicial del tetris.
 * @param base_level Puntero al primer nodo de los niveles.
 * @param thread_count Cantidad de hilos.
 * @return void.
 */
void solve_tetris_omp(tetris_t* base_tetris, struct level_t* base_level,
                  int thread_count);

/**
 * @brief Rutina para iniciar la solución del tetris.
 * @details Recorre el nivel 0 y llama a la rutina del nivel 1.
 * @param tetris Puntero al estado inicial del tetris.
 * @param thread_count Cantidad de hilos.
 * @return void.
 */
void solve_tetris_mpi(tetris_t* tetris, int thread_count);

#endif
