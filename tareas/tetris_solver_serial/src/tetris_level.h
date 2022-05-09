// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_LEVEL_H
#define TETRIS_LEVEL_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estructura para manejo de los niveles.
 * @details Contiene la letra de la figura, su rotación y la matriz de posiciones.
 */
typedef struct {
    char figure;
    int shape;
    char** matrix;
} level_t;

/**
 * @brief Rutina para guardar un nivel.
 * @details Almacena la información correspondiente a un nivel en el puntero recibido.
 * @param level Puntero donde se guardará la información.
 * @param figure Letra de la figura colocada en ese nivel.
 * @param shape Rotación de la figura colocada en ese nivel.
 * @param num_rows Cantidad de filas de la matriz.
 * @param num_cols Cantidad de columnas de la matriz.
 * @param matrix Puntero a la matriz del estado actual del juego que será almacenada en el nivel.
 * @return Un valor entero: 0 en caso de error, 1 en caso de éxito.
 */
int save_level(level_t* level, char figure, int shape, int num_rows,
        int num_cols, char** matrix);

#endif
