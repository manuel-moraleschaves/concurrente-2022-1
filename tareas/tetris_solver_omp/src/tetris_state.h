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
 * @brief Rutina destruir el estado del tetris.
 * @details Elimina y libera los datos almacenados en memoria del estado del tetris.
 * @param tetris Puntero al estado del tetris.
 * @return Void.
 */
void destroy_tetris(tetris_t* tetris);

/**
 * @brief Rutina para clonar una estado del tetris.
 * @details Crea un nuevo tetris y clona los datos del estado origen.
 * @param base_tetris Puntero al estado del tetris inicial.
 * @return Un puntero al nuevo tetris_t* creado.
 */
tetris_t* clone_tetris(tetris_t* base_tetris);

#endif
