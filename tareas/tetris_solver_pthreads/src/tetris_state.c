// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"
#include "tetris_figure_factory.h"
#include "tetris_general.h"

tetris_t* read_tetris(FILE* file) {
    tetris_t* tetris = malloc(sizeof(tetris_t));

    if ( tetris == NULL ) {
        return NULL;
    }

    // Lectura del id
    if (fscanf(file, "%zu", &tetris->id) == 0 || tetris->id <= 0) {
        fprintf(stderr, "Error: invalid identifier in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("ID: %zu\n", tetris->id);

    // Lectura de la profundidad
    if (fscanf(file, "%i", &tetris->depth) == 0 || tetris->depth < 0) {
        fprintf(stderr, "Error: invalid depth in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Depth: %d\n", tetris->depth);

    // Lectura de la cantidad de filas
    if (fscanf(file, "%i", &tetris->rows) == 0 || tetris->rows <= 0) {
        fprintf(stderr, "Error: invalid rows in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Rows: %d\n", tetris->rows);

    // Lectura de la cantidad de columnas
    if (fscanf(file, "%i", &tetris->columns) == 0 || tetris->columns <= 0) {
        fprintf(stderr, "Error: invalid columns in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Columns: %d\n", tetris->columns);

    // Creación de la matriz
    tetris->matrix = (char**)create_matrix(tetris->rows,
                        tetris->columns + 1, sizeof(char));

    if (!tetris->matrix) {
        fprintf(stderr, "Error: could not create the tetris matrix.\n");
        free(tetris);
        return NULL;
    }

    // Lectura de los datos de la matriz
    for (int i = 0; i < tetris->rows; ++i) {
         fscanf(file, "%s", tetris->matrix[i]);
    }

    print_matrix(tetris->rows, tetris->matrix);

    // Lectura del contador de figuras cayendo
    if (fscanf(file, "%i", &tetris->sequence_count) == 0
        || tetris->sequence_count <= 0
        || tetris->sequence_count < tetris->depth+1) {
        fprintf(stderr, "Error: invalid sequence count in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("K: %d\n", tetris->sequence_count);

    // Creación de la secuencia de figuras
    tetris->figure_sequence = (char*) calloc(tetris->sequence_count + 1,
                                sizeof(char));

    if (!tetris->figure_sequence) {
        fprintf(stderr, "Error: could not create the figure sequence.\n");
        free(tetris);
        return NULL;
    }

    // Se utiliza depth para cargar sola la secuencia que se va a usar
    for (int i = 0; i < tetris->depth+1; ++i) {
        getc(file);
        tetris->figure_sequence[i] = getc(file);
    }
    tetris->figure_sequence[tetris->sequence_count] = '\0';
    printf("Figure sequence: %s\n", tetris->figure_sequence);

    // Creación de los niveles
    tetris->levels = create_level('B', 0, tetris->rows, tetris->columns,
                                  tetris->matrix);

    if (!tetris->levels) {
        fprintf(stderr, "Error: could not create the levels.\n");
        free(tetris);
        return NULL;
    }

    // Inicialización de la altura mínima
    tetris->min_height = tetris->rows + 1;

    return tetris;
}

void destroy_tetris(tetris_t* tetris) {
    printf("\nDENTRO DE destroy_tetris...\n");

    free_matrix(tetris->rows, (void**)tetris->matrix);
    printf(" liberada la matrix...\n");

    destroy_levels(tetris->levels, tetris->rows);
    printf(" liberado los niveles...\n");

    free(tetris->figure_sequence);
    printf(" liberado figure_sequence...\n");

    free(tetris);
    printf(" liberado todo tetris...\n");
}

int solve_tetris_dfs(tetris_t* tetris, int piece_index,
                     struct level_t* base_level) {
    // Si es el último nivel
    if (piece_index == tetris->depth + 1) {
        int current_height = calculate_height(tetris);
        // Se actualiza la altura mínima alcanzada
        if (current_height < tetris->min_height) {
            tetris->min_height = current_height;

            // Clona la secuencia de niveles hacia el estado del tetris
            if (base_level) {
                clone_level(base_level, tetris->levels, tetris->rows,
                            tetris->columns);
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
            // TODO(manum): optimizar
            if (calculate_height(tetris) > tetris->min_height) {
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
                                              base_level);

                    // Remueve la figura colocada
                    remove_figure(tetris, figure, num_row, num_col);
                }
            }
        }
    }
    return result;
}
