// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"
#include "tetris_figure_factory.h"

int calculate_height(tetris_t* tetris);
int valid_column(tetris_t* tetris, figure_t* figure, int num_col/*, FILE* file*/);
int place_figure(tetris_t* tetris, figure_t* figure, int num_col/*, FILE* file*/);
int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col/*, FILE* file*/);
void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col/*, FILE* file*/);
void remove_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);


tetris_t* read_tetris(FILE* file) {
    tetris_t* tetris = malloc(sizeof(tetris_t));

    if (fscanf(file, "%zu", &tetris->id) == 0 || tetris->id <= 0) {
        fprintf(stderr, "Error: invalid identifier in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("ID: %zu\n", tetris->id);

    if (fscanf(file, "%i", &tetris->depth) == 0 || tetris->depth < 0) {
        fprintf(stderr, "Error: invalid depth in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Depth: %d\n", tetris->depth);

    if (fscanf(file, "%i", &tetris->rows) == 0 || tetris->rows <= 0) {
        fprintf(stderr, "Error: invalid rows in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Rows: %d\n", tetris->rows);

    if (fscanf(file, "%i", &tetris->columns) == 0 || tetris->columns <= 0) {
        fprintf(stderr, "Error: invalid columns in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("Columns: %d\n", tetris->columns);

    tetris->matrix = (char**)create_matrix(tetris->rows,
                        tetris->columns + 1, sizeof(char));

    if (!tetris->matrix) {
        fprintf(stderr, "Error: could not create the tetris matrix.\n");
        free(tetris);
        return NULL;
    }

    for (int i = 0; i < tetris->rows; ++i) {
         fscanf(file, "%s", tetris->matrix[i]);
    }

    print_matrix(tetris->rows, tetris->matrix);

    if (fscanf(file, "%i", &tetris->sequence_count) == 0
        || tetris->sequence_count <= 0
        || tetris->sequence_count < tetris->depth+1) {
        fprintf(stderr, "Error: invalid sequence count in the file.\n");
        free(tetris);
        return NULL;
    }
    printf("K: %d\n", tetris->sequence_count);

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

    // tetris->levels = (level_t*) calloc(tetris->depth, sizeof(level_t));

    // if (!tetris->levels) {
    //     fprintf(stderr, "Error: could not create the levels.\n");
    //     free(tetris);
    //     return NULL;
    // }

    tetris->min_height = tetris->rows;

    return tetris;
}

void destroy_tetris(tetris_t* tetris) {
    free_matrix(tetris->rows, (void**)tetris->matrix);
    // for (int i = 0; i < tetris->depth; ++i) {
    //     free_matrix(tetris->rows, (void**)tetris->levels->matrix);
    // }
    // free(tetris->levels);
    free(tetris->figure_sequence);
    free(tetris);
}

int solve_tetris_dfs(tetris_t* tetris, int piece_index, FILE* file) {
    fprintf(file, "\n\n**INICIO DFS**\npiece: %c - piece_index: %d\n",
        tetris->figure_sequence[piece_index], piece_index);

    if (piece_index == tetris->depth + 1) {
        // fprintf(file, "  Dentro de if (piece_index == tetris->depth + 1)\n");
        int current_height = calculate_height(tetris);
        // fprintf(file, "    height: %d - min_height: %d\n", current_height, tetris->min_height);
        if (current_height < tetris->min_height) {
            // fprintf(file, "    Dentro de if (current_height < tetris->min_height)\n");
            tetris->min_height = current_height;
            // fprintf(file, "      min_height: %d\n", tetris->min_height);
            // fprintf(file, "      return...\n");
            return 1;
        } else {
            // fprintf(file, "    Por el else de (current_height < tetris->min_height)\n");
            // fprintf(file, "      return...\n");
            return 0;
        }
    }

    int num_rotations =
        get_tetris_figure_num_rotations(tetris->figure_sequence[piece_index]);
    // fprintf(file, "num_rotations: %i\n", num_rotations);
    int result = 0;

    for (int rotation = 0; rotation < num_rotations; ++rotation) {
        // fprintf(file, "ROTATION: %i\n", rotation);
        figure_t* figure =
            get_tetris_figure(tetris->figure_sequence[piece_index], rotation);
        // fprintf(file, "FIGURE: %s\n", figure->value[0]);

        for (int num_col = 0; num_col < tetris->columns; ++num_col) {
            // fprintf(file, "num_col: %i\n", num_col);
            // int current = calculate_height(tetris);
            // fprintf(file, "current:%i >= min_height: %i\n", current, tetris->min_height);
            if (calculate_height(tetris) > tetris->min_height) {
                fprintf(file, "va a continuar...\n");
                continue;
            }
            // fprintf(file, "despues del continue:...\n");

            if (valid_column(tetris, figure, num_col/*, file*/)) {
                // fprintf(file, "al salir de valid_column: ADENTRO...\n");

                // TODO(manum): SI NO LA PUDO PONER TERMINAR TODO
                int num_row = place_figure(tetris, figure, num_col/*, file*/);
                print_matrix2(tetris->rows, tetris->matrix, file);

                if (num_row != -1) {
                    result = solve_tetris_dfs(tetris, piece_index + 1, file);
                    // fprintf(file, "  resultado dfs: %i\n", result);

                    // if (result == 1) {
                    //    tetris->levels =
                    //        save_level(tetris->figure_sequence[piece_index],
                    //                   rotation, tetris->rows, tetris->columns,
                    //                   tetris->matrix);
                    // }
                    remove_figure(tetris, figure, num_row, num_col);
                    print_matrix2(tetris->rows, tetris->matrix, file);
                    fprintf(file, "----------\n");
                }
            }
            // fprintf(file, "despues de valid_column...\n");
        }
    }

    return result;
}

int calculate_height(tetris_t* tetris) {
    for (int i = 0; i < tetris->rows; ++i) {
        // int piece_in_row = 0;

        for (int j = 0; j < tetris->columns; ++j) {
            if (tetris->matrix[i][j] != '0') {
                // piece_in_row = 1;
                // break;
                return tetris->rows - i;
            }
        }

        // if (piece_in_row == 0) {
        //    printf("height1: %i\n", i);
        //    return i;
        // }
    }

    // printf("height2: %i\n", tetris->rows);
    return tetris->rows;
}

int valid_column(tetris_t* tetris, figure_t* figure, int num_col/*, FILE* file*/) {
    // fprintf(file, "DENTRO DE valid_column...\n");
    // fprintf(file, " num_col:%i + figure->width:%i = %i\n", num_col, figure->width, num_col + figure->width);
    if ((num_col < 0) || ((num_col + figure->width) > tetris->columns)) {
        // fprintf(file, "   dentro del if, retorno 0...\n");
        return 0;
    } else {
        // fprintf(file, "   por el else, retorno 1...\n");
        return 1;
    }
}

int place_figure(tetris_t* tetris, figure_t* figure, int num_col/*, FILE* file*/) {
    // fprintf(file, "DENTRO DE place_figure...\n");
    int x = 0;
    int num_row = -1;

    while (x >= 0) {
        // fprintf(file, " x: %i\n", x);
        // fprintf(file, " num_row: %i\n", num_row);
        // TODO(manum): REVISAR
        // if ((x + figure->height > tetris->rows) ||
        if ((x + figure->height <= tetris->rows) &&
            !figure_collides(tetris, figure, x, num_col/*, file*/))  {
            // fprintf(file, " dentro del if sin colisiones...\n");
            num_row = x;
            x++;
        } else {
            break;
        }
    }
    // fprintf(file, " num_row: %i\n", num_row);

    if (num_row >= 0) {
        // fprintf(file, "  dentro de if num_row >= 0...\n");
        insert_figure(tetris, figure, num_row, num_col/*, file*/);
        // fprintf(file, "  return %i\n", num_row);
        // return num_row;
    }  // else {
        // fprintf(file, "  por el else de num_row >= 0...\n");
        // fprintf(file, "  return -1\n");
        // return -1;
    // }
    return num_row;
}

int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col/*, FILE* file*/) {
    // fprintf(file, "DENTRO DE figure_collides...\n");
    // fprintf(file, " num_row: %i - num_col: %i\n", num_row, num_col);

    for (int i = 0; i < figure->height; ++i) {
        // fprintf(file, "  i: %i - ", i);
        for (int j = 0; j < figure->width; ++j) {
            // fprintf(file, "  j: %i\n", j);
            // fprintf(file, "  matrix[%i][%i]: %c\n", num_row + i, num_col + j, tetris->matrix[num_row + i][num_col + j]);
            // fprintf(file, "  value[%i][%i]: %c\n", i, j, figure->value[i][j]);
            if (((char)tetris->matrix[num_row + i][num_col + j] != '0') &&
                ((char)figure->value[i][j] != '0')) {
                    // fprintf(file, "   dentro del if...\n");
                    // fprintf(file, "   retorno 1...\n");
                    return 1;
            }
            // fprintf(file, "  paso el if...\n");
        }
    }
    // fprintf(file, " retorno 0...\n");
    return 0;
}

void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col/*, FILE* file*/) {
    // fprintf(file, "DENTRO DE insert_figure...\n");
    for (int i = 0; i < figure->height; ++i) {
        for (int j = 0; j < figure->width; ++j) {
            // fprintf(file, "  matrix[%i][%i]: %c\n", num_row + i, num_col + j, tetris->matrix[num_row + i][num_col + j]);
            if (figure->value[i][j] != '0') {
                tetris->matrix[num_row + i][num_col + j] = figure->value[i][j];
            }
            // fprintf(file, "  value[%i][%i]: %c\n", i, j, figure->value[i][j]);
            // fprintf(file, "  matrix[%i][%i]: %c\n", num_row + i, num_col + j, tetris->matrix[num_row + i][num_col + j]);
        }
    }
}

void remove_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
    for (int i = 0; i < figure->height; ++i) {
        for (int j = 0; j < figure->width; ++j) {
            if (figure->value[i][j] != '0') {
                tetris->matrix[num_row + i][num_col + j] = '0';
            }
        }
    }
}
