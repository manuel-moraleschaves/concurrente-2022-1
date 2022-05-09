// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"
#include "tetris_figure_factory.h"

int calculate_height(tetris_t* tetris);
int valid_column(tetris_t* tetris, figure_t* figure, int num_col);
int place_figure(tetris_t* tetris, figure_t* figure, int num_co);
int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);
void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);
void remove_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);


tetris_t* read_tetris(FILE* file) {
    tetris_t* tetris = malloc(sizeof(tetris_t));

    if ( tetris == NULL ) {
        return NULL;
    }

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

    tetris->levels = (level_t*) calloc(tetris->depth + 1, sizeof(level_t));

    if (!tetris->levels) {
        fprintf(stderr, "Error: could not create the levels.\n");
        free(tetris);
        return NULL;
    }

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

    if (piece_index == tetris->depth + 1) {
        int current_height = calculate_height(tetris);
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

    for (int rotation = 0; rotation < num_rotations; ++rotation) {
        figure_t* figure =
            get_tetris_figure(tetris->figure_sequence[piece_index], rotation);

        for (int num_col = 0; num_col < tetris->columns; ++num_col) {
            // TODO(manum): optimizar
            if (calculate_height(tetris) > tetris->min_height) {
                fprintf(file, "va a continuar...\n");
                continue;
            }

            if (valid_column(tetris, figure, num_col)) {
                int num_row = place_figure(tetris, figure, num_col);

                if (num_row != -1) {
                    fprintf(file, "\n  PIEZA COLOCADA!\n");
                    print_matrix_file(tetris->rows, tetris->matrix, file);

                    result = solve_tetris_dfs(tetris, piece_index + 1, file);
                    fprintf(file, "  resultado dfs para el %i: %i\n",
                        piece_index, result);

                    if (result == 1) {
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
                    remove_figure(tetris, figure, num_row, num_col);
                    fprintf(file, "  PIEZA REMOVIDA!\n");
                    print_matrix_file(tetris->rows, tetris->matrix, file);
                    fprintf(file, "----------\n");
                    if (result == 1 && piece_index > 0) {
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
    if ((num_col < 0) || ((num_col + figure->width) > tetris->columns)) {
        return 0;
    } else {
        return 1;
    }
}

int place_figure(tetris_t* tetris, figure_t* figure, int num_col) {
    int x = 0;
    int num_row = -1;

    while (x >= 0) {
        if ((x + figure->height <= tetris->rows) &&
            !figure_collides(tetris, figure, x, num_col))  {
            num_row = x;
            x++;
        } else {
            break;
        }
    }

    if (num_row >= 0) {
        insert_figure(tetris, figure, num_row, num_col);
    }
    return num_row;
}

int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
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
    for (int i = 0; i < figure->height; ++i) {
        for (int j = 0; j < figure->width; ++j) {
            if (figure->value[i][j] != '0') {
                tetris->matrix[num_row + i][num_col + j] = '0';
            }
        }
    }
}
