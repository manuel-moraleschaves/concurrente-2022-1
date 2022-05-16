// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_general.h"

int calculate_height(tetris_t* tetris) {
    // Se recorren las filas para verificar la altura actual
    for (int i = 0; i < tetris->rows; ++i) {
        for (int j = 0; j < tetris->columns; ++j) {
            if (tetris->matrix[i][j] != '0') {
                return tetris->rows - i;
            }
        }
    }

    return tetris->rows;
}

int valid_column(tetris_t* tetris, figure_t* figure, int num_col) {
    // Se valida la columna para que la figura no se salga del tablero
    if ((num_col < 0) || ((num_col + figure->width) > tetris->columns)) {
        return 0;
    } else {
        return 1;
    }
}

int place_figure(tetris_t* tetris, figure_t* figure, int num_col) {
    int x = 0;
    int num_row = -1;

    // Baja por las filas hasta encontrar una donde no colisione
    while (x >= 0) {
        if ((x + figure->height <= tetris->rows) &&
            !figure_collides(tetris, figure, x, num_col))  {
            num_row = x;
            x++;
        } else {
            break;
        }
    }

    // Si encontró una fila donde colocar la figura, la inserta
    if (num_row >= 0) {
        insert_figure(tetris, figure, num_row, num_col);
    }

    return num_row;
}

int figure_collides(tetris_t* tetris, figure_t* figure,
    int num_row, int num_col) {
    // Recorre las columnas y filas para encontrar colisiones
    for (int i = 0; i < figure->height; ++i) {
        for (int j = 0; j < figure->width; ++j) {
            if (((char)tetris->matrix[num_row + i][num_col + j] != '0') &&
                ((char)figure->value[i][j] != '0')) {
                    return 1;
            }
        }
    }

    return 0;
}

void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
    // Inserta la figura en la posición indicada
    for (int i = 0; i < figure->height; ++i) {
        for (int j = 0; j < figure->width; ++j) {
            if (figure->value[i][j] != '0') {
                tetris->matrix[num_row + i][num_col + j] = figure->value[i][j];
            }
        }
    }
}

void remove_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
    // Remueve la figura de la posición indicada
    for (int i = 0; i < figure->height; ++i) {
        for (int j = 0; j < figure->width; ++j) {
            if (figure->value[i][j] != '0') {
                tetris->matrix[num_row + i][num_col + j] = '0';
            }
        }
    }
}
