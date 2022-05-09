// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"
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
int place_figure(tetris_t* tetris, figure_t* figure, int num_co);

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
    tetris->levels = (level_t*) calloc(tetris->depth + 1, sizeof(level_t));

    if (!tetris->levels) {
        fprintf(stderr, "Error: could not create the levels.\n");
        free(tetris);
        return NULL;
    }

    // Inicialización de la altura mínima
    tetris->min_height = tetris->rows;

    return tetris;
}

void destroy_tetris(tetris_t* tetris) {
    free_matrix(tetris->rows, (void**)tetris->matrix);
    for (int i = 0; i <= tetris->depth; ++i) {
        free_matrix(tetris->rows, (void**)tetris->levels[i].matrix);
    }
    free(tetris->levels);
    free(tetris->figure_sequence);
    free(tetris);
}

int solve_tetris_dfs(tetris_t* tetris, int piece_index, FILE* file) {
    fprintf(file, "\n\n**INICIO DFS**\npiece: %c - piece_index: %d\n",
        tetris->figure_sequence[piece_index], piece_index);

    // Si es el último nivel
    if (piece_index == tetris->depth + 1) {
        int current_height = calculate_height(tetris);
        // Se actualiza la altura mínima alcanzada
        if (current_height < tetris->min_height) {
            tetris->min_height = current_height;
            fprintf(file, "saliendo DFS con %i...\n\n", 1);
            return 1;
        } else {
            fprintf(file, "saliendo DFS con %i...\n\n", 0);
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
                fprintf(file, "va a continuar...\n");
                continue;
            }

            // Se valida si la figura cabe en la columna
            if (valid_column(tetris, figure, num_col)) {
                // Intenta colocar la figura en la fila más baja
                int num_row = place_figure(tetris, figure, num_col);

                // Si logró colocar la figura
                if (num_row != -1) {
                    fprintf(file, "\n  PIEZA COLOCADA!\n");
                    print_matrix_file(tetris->rows, tetris->matrix, file);

                    // Llamado recursivo con la siguiente figura
                    result = solve_tetris_dfs(tetris, piece_index + 1, file);
                    fprintf(file, "  resultado dfs para el %i: %i\n",
                        piece_index, result);

                    // Si obtuvo un mejor puntaje (nueva altura mínima)
                    if (result == 1) {
                        // Guarda el estado actual
                        result = save_level(&tetris->levels[piece_index],
                                    tetris->figure_sequence[piece_index],
                                    rotation, tetris->rows, tetris->columns,
                                    tetris->matrix);

                        if (result == 1) {
                            fprintf(file, "  level %i saved!\n",
                                piece_index);
                            print_matrix_file(tetris->rows,
                                tetris->levels[piece_index].matrix, file);
                            fprintf(file, "----------\n");
                        }
                    }
                    // Remueve la figura colocada
                    remove_figure(tetris, figure, num_row, num_col);
                    fprintf(file, "  PIEZA REMOVIDA!\n");
                    print_matrix_file(tetris->rows, tetris->matrix, file);
                    fprintf(file, "----------\n");
                    if (result == 1 && piece_index > 0) {
                        // Guarda el estado actual para el nivel anterior
                        result = save_level(&tetris->levels[piece_index-1],
                                    tetris->figure_sequence[piece_index],
                                    rotation, tetris->rows, tetris->columns,
                                    tetris->matrix);

                        if (result == 1) {
                            fprintf(file, "  level %i saved!\n",
                                    piece_index-1);
                            print_matrix_file(tetris->rows,
                                tetris->levels[piece_index-1].matrix, file);
                            fprintf(file, "----------\n");
                        }
                    }
                } else {
                    fprintf(file, " NO SE PUDO COLOCAR LA PIEZA!\n");
                }
            }
        }
    }
    fprintf(file, "saliendo DFS con %i...\n\n", result);
    return result;
}

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
