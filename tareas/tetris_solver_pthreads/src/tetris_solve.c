// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_solve.h"
#include "tetris_figure_factory.h"
#include "tetris_general.h"

int solve_tetris_dfs(tetris_t* tetris, int piece_index,
                     struct level_t* base_level, shared_data_t* shared_data) {
    // Si es el último nivel
    if (piece_index == tetris->depth + 1) {
        int current_height = calculate_height(tetris);

        // Se actualiza la altura mínima alcanzada
        pthread_mutex_lock(&shared_data->can_access_min_height);

        if (current_height < shared_data->tetris->min_height) {
            shared_data->tetris->min_height = current_height;
            pthread_mutex_unlock(&shared_data->can_access_min_height);
        } else {
            pthread_mutex_unlock(&shared_data->can_access_min_height);
            return 0;
        }

        // Clona la secuencia de niveles hacia el estado del tetris
        if (base_level) {
            pthread_mutex_lock(&shared_data->can_access_levels);
            clone_level(base_level, shared_data->tetris->levels,
                        tetris->rows, tetris->columns);
            pthread_mutex_unlock(&shared_data->can_access_levels);
        }
        return 1;
    }

    int num_rotations =
        get_tetris_figure_num_rotations(tetris->figure_sequence[piece_index]);
    int result = 0;

    // Se recorren todas las posibles rotaciones de la figura
    for (int rotation = 0; rotation < num_rotations; ++rotation) {
        figure_t* figure =
            get_tetris_figure(tetris->figure_sequence[piece_index], rotation);

        // Se recorren todas las columnas del tablero
        for (int num_col = 0; num_col < tetris->columns; ++num_col) {
            pthread_mutex_lock(&shared_data->can_access_min_height);
            int shared_min_height = shared_data->tetris->min_height;
            pthread_mutex_unlock(&shared_data->can_access_min_height);

            if (calculate_height(tetris) > shared_min_height) {
                continue;
            }

            // Se valida si la figura cabe en la columna
            if (valid_column(tetris, figure, num_col)) {
                // Intenta colocar la figura en la fila más baja
                int num_row = place_figure(tetris, figure, num_col);

                // Si logró colocar la figura
                if (num_row != -1) {
                    // Crea el registro del nivel
                    struct level_t* level =
                        create_level(tetris->figure_sequence[piece_index],
                                     rotation, tetris->rows, tetris->columns,
                                     tetris->matrix);
                    if (level) {
                        // Llegar al nodo correspondiente al nivel
                        struct level_t * current = base_level;
                        for (int i = 0; i < piece_index; ++i) {
                            current = current->next;
                        }

                        // Si tiene más niveles siguientes los elimina
                        if (current->next) {
                            destroy_levels(current->next, tetris->rows);
                        }

                        // Agrega el nuevo nivel
                        current->next = level;
                        current->next->next = NULL;
                    }

                    // Llamado recursivo con la siguiente figura
                    result = solve_tetris_dfs(tetris, piece_index + 1,
                                              base_level, shared_data);

                    // Remueve la figura colocada
                    remove_figure(tetris, figure, num_row, num_col);
                }
            }
        }
    }
    return result;
}

void *solve_tetris(void *data) {
    const private_data_t* private_data = (private_data_t*)data;
    shared_data_t* shared_data = private_data->shared_data;

    int num_rotations =
       get_tetris_figure_num_rotations(shared_data->tetris->figure_sequence[0]);

    // Se recorren las unidades de trabajo (columnas) segun un mapeo ciclico
    for (int col = private_data->thread_number;
                        col < num_rotations * shared_data->tetris->columns;
                        col += shared_data->thread_count) {
        for (int rotation = 0; rotation < num_rotations; ++rotation) {
            // Se clona el tetris
            tetris_t* tetris = clone_tetris(shared_data);

            figure_t* figure = get_tetris_figure(tetris->figure_sequence[0],
                                                 rotation);

            // Se valida si la figura cabe en la columna
            if (valid_column(tetris, figure, col)) {
                // Intenta colocar la figura en la fila más baja
                int num_row = place_figure(tetris, figure, col);

                pthread_mutex_lock(&shared_data->can_access_min_height);
                int shared_min_height = shared_data->tetris->min_height;
                pthread_mutex_unlock(&shared_data->can_access_min_height);

                if (calculate_height(tetris) > shared_min_height) {
                    destroy_tetris(tetris);
                    continue;
                }

                // Si logró colocar la figura
                if (num_row != -1) {
                    // Crea el registro del nivel
                    struct level_t* level =
                                        create_level(tetris->figure_sequence[0],
                                        rotation, tetris->rows, tetris->columns,
                                        tetris->matrix);
                    if (level) {
                        struct level_t * current = tetris->levels;

                        // Agrega el nuevo nivel
                        current->next = level;
                        current->next->next = NULL;
                    }

                    // Llamado a rutina recursiva con la siguiente figura
                    solve_tetris_dfs(tetris, 1, tetris->levels, shared_data);
                }
            }
            // Liberar tetris
            destroy_tetris(tetris);
        }
    }
    return NULL;
}
