// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_solve.h"
#include "tetris_figure_factory.h"
#include "tetris_general.h"
#include "omp.h"

int solve_tetris_dfs(tetris_t* tetris, int piece_index,
                     struct level_t* base_level, tetris_t* base_tetris) {
    // Si es el último nivel
    if (piece_index == tetris->depth + 1) {
        int current_height = calculate_height(tetris);

        // Se actualiza la altura mínima alcanzada
        int clone = 0;
        #pragma omp critical(height)
        {
            if (current_height < base_tetris->min_height) {
                base_tetris->min_height = current_height;
                clone = 1;
            }
        }

        if (clone == 1) {
            // Clona la secuencia de niveles hacia el estado del tetris
            if (base_level) {
                #pragma omp critical(level)
                {
                    clone_level(base_level, base_tetris->levels,
                                tetris->rows, tetris->columns);
                }
            }
            return 1;
        } else {
            return 0;
        }
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
            int shared_min_height = 0;
            #pragma omp critical(height)
            {
                shared_min_height = base_tetris->min_height;
            }

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
                                              base_level, base_tetris);

                    // Remueve la figura colocada
                    remove_figure(tetris, figure, num_row, num_col);
                }
            }
        }
    }
    return result;
}

void solve_tetris(tetris_t* base_tetris, int thread_count) {
    int num_rotations =
       get_tetris_figure_num_rotations(base_tetris->figure_sequence[0]);

    // Se recorren las unidades de trabajo (columnas) segun un mapeo ciclico
    #pragma omp parallel for num_threads(thread_count) default(none) \
         shared(base_tetris, num_rotations) schedule(static, 1)
    for (int col = 0; col < base_tetris->columns; col++) {
        for (int rotation = 0; rotation < num_rotations; ++rotation) {
            // Se clona el tetris
            tetris_t* tetris = NULL;
            #pragma omp critical(clone)
            {
                tetris = clone_tetris(base_tetris);
            }

            figure_t* figure = get_tetris_figure(tetris->figure_sequence[0],
                                                 rotation);

            // Se valida si la figura cabe en la columna
            if (valid_column(tetris, figure, col)) {
                // Intenta colocar la figura en la fila más baja
                int num_row = place_figure(tetris, figure, col);

                int shared_min_height = 0;
                #pragma omp critical(height)
                {
                    shared_min_height = base_tetris->min_height;
                }

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
                    solve_tetris_dfs(tetris, 1, tetris->levels, base_tetris);
                }
            }
            // Liberar tetris
            destroy_tetris(tetris);
        }
    }
}
