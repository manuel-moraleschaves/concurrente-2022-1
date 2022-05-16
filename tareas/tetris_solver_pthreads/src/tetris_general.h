// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_GENERAL_H
#define TETRIS_GENERAL_H

#include <stdio.h>
#include <stdlib.h>

#include "tetris_state.h"
#include "tetris_figure_factory.h"

/**
 * @brief Rutina para calcular la altura.
 * @details Calcula la altura del estado actual del tetris.
 * @param tetris Puntero al estado del tetris.
 * @return Un valor entero que representa la altura calculada.
 */
int calculate_height(tetris_t* tetris);

/**
 * @brief Rutina para validar una columna.
 * @details Verifica si una figura se puede colocar en la columna indicada.
 * @param tetris Puntero al estado del tetris.
 * @param figure Puntero a la figura correspondiente.
 * @param num_col Número de la columna que se quiere validar.
 * @return Un valor entero: 0 en caso de error, 1 en caso de éxito.
 */
int valid_column(tetris_t* tetris, figure_t* figure, int num_col);

/**
 * @brief Rutina para colocar una figura.
 * @details Intenta colocar una figura a partir de la columna dada.
 * @param tetris Puntero al estado del tetris.
 * @param figure Puntero a la figura correspondiente.
 * @param num_col Número de la columna donde se quiere colocar la figura.
 * @return Un valor entero que representa el número de fila donde se insertó la
 * figura, o un -1 en caso de no haber logrado la inserción.
 */
int place_figure(tetris_t* tetris, figure_t* figure, int num_col);

/**
 * @brief Rutina para encontrar las colisiones de una figura.
 * @details A partir de la fila dada, verifica cada columna para determinar si
 * hay alguna colisión con otra figura.
 * @param tetris Puntero al estado del tetris.
 * @param figure Puntero a la figura correspondiente.
 * @param num_row Número de la fila donde se quiere colocar la figura.
 * @param num_col Número de la columna donde se quiere colocar la figura.
 * @return Un valor entero: 0 en caso de no haber colisiones, 1 en caso de
 * encontrar alguna colisión.
 */
int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);

/**
 * @brief Rutina para insertar una figura.
 * @details Inserta la figura a partir de la fila y columna dadas.
 * @param tetris Puntero al estado del tetris.
 * @param figure Puntero a la figura correspondiente.
 * @param num_row Número de la fila donde se quiere insertar la figura.
 * @param num_col Número de la columna donde se quiere insertar la figura.
 * @return void.
 */
void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);

/**
 * @brief Rutina para remover una figura.
 * @details Remueve la figura a partir de la fila y columna dadas.
 * @param tetris Puntero al estado del tetris.
 * @param figure Puntero a la figura correspondiente.
 * @param num_row Número de la fila donde se quiere remover la figura.
 * @param num_col Número de la columna donde se quiere remover la figura.
 * @return void.
 */
void remove_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);

#endif
