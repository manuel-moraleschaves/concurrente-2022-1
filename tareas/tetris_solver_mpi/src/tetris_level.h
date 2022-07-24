// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_LEVEL_H
#define TETRIS_LEVEL_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Estructura tipo cola para manejo de los niveles.
 * @details Contiene la letra de la figura, su rotación, la matriz de posiciones y un puntero al nivel siguiente.
 */
struct level_t {
    char figure;
    int rotation;
    char** matrix;
    struct level_t* next;
};

/**
 * @brief Rutina para crear un nivel.
 * @details Crea un nuevo nivel con los datos del estado del tetris.
 * @param figure Letra de la figura colocada en el nivel.
 * @param rotation rotación de la figura colocada en el nivel.
 * @param num_rows Cantidad de filas de la matriz.
 * @param num_cols Cantidad de columnas de la matriz.
 * @param matrix Matriz con los datos de las figuras que tiene el nivel.
 * @return Puntero al nivel creado.
 */
struct level_t* create_level(char figure, int rotation, int num_rows,
                             int num_cols, char** matrix);

/**
 * @brief Rutina para clonar un nivel.
 * @details Clona los datos de un nivel origen a un nivel de destino.
 * @param source Puntero al nivel con los datos de origen.
 * @param dest Puntero al nivel de destino al que se clonarán los datos.
 * @param num_rows Cantidad de filas de la matriz.
 * @param num_cols Cantidad de columnas de la matriz.
 * @return void.
 */
void clone_level(struct level_t* source, struct level_t* dest,
                 int num_rows, int num_cols);

/**
 * @brief Rutina para destruir los niveles.
 * @details Destruye un nodo base y todos sus nodos siguientes, así como sus matrices.
 * @param source Puntero al nivel base.
 * @param num_rows Cantidad de filas de la matriz.
 * @return void.
 */
void destroy_levels(struct level_t* source , int num_rows);

#endif
