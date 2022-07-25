// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#ifndef TETRIS_FILE_OPS_H
#define TETRIS_FILE_OPS_H

#include <stdio.h>
#include <stdlib.h>

#include "tetris_state.h"

/**
 * @brief Rutina para leer el estado inicial del tetris.
 * @details Lee la información del archivo de entrada y crea el estado inicial del tetris.
 * @param file Puntero al archivo de entrada.
 * @param rank Número del proceso actual.
 * @return Un puntero al estado inicial creado o NULL en caso de error.
 */
tetris_t* read_tetris(FILE* file, int rank);

/**
 * @brief Rutina para generar los archivos finales.
 * @details Genera los archivos finales correspondientes a cada nivel en el que se pudo colocar una pieza
 * @param tetris Puntero al estado del tetris.
 * @param base_level Puntero al nodo base de los niveles.
 * @return void.
 */
void generate_files(tetris_t* tetris, struct level_t* base_level);

#endif
