// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_STATE_H
#define TETRIS_STATE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "tetris_level.h"
#include "tetris_figure_factory.h"

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

typedef struct {
    int rotation;
    int column;
} possible_plays_t;

typedef struct {
    size_t thread_count;
    int plays_count;
    possible_plays_t* plays;
    tetris_t* tetris;
    pthread_mutex_t can_access_min_height;
    pthread_mutex_t can_access_levels;
} shared_data_t;

typedef struct {
    size_t thread_number;
    shared_data_t *shared_data;
} private_data_t;




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
                     struct level_t* base_level, shared_data_t* shared_data);

/**
 * @brief Rutina para iniciar la solución del tetris.
 * @details Recorre el nivel 0 y llama a la rutina recursiva dfs.
 * @param data Puntero a los datos privados del hilo.
 * @return void.
 */
void *solve_tetris(void *data);


tetris_t* clone_tetris(shared_data_t* shared_data);

#endif
