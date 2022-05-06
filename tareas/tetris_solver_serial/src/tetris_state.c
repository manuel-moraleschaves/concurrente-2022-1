// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_state.h"
#include "tetris_utils.h"
#include "tetris_figure_factory.h"

int calculate_height(tetris_t* tetris);
int valid_column(tetris_t* tetris, figure_t* figure, int num_col);
int place_figure(tetris_t* tetris, figure_t* figure, int num_col);
int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);
void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col);
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

    if (fscanf(file, "%i", &tetris->depth) == 0 || tetris->depth <= 0) {
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

    print_matrix(tetris->rows, (void**)tetris->matrix);

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

    for (int i = 0; i < tetris->sequence_count; ++i) {
        getc(file);
        tetris->figure_sequence[i] = getc(file);
    }
    tetris->figure_sequence[tetris->sequence_count] = '\0';
    printf("Figure sequence: %s\n", tetris->figure_sequence);

    tetris->levels = (level_t*) calloc(tetris->depth, sizeof(level_t));
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
    for (int i = 0; i < tetris->depth; i++) {
        free_matrix(tetris->rows, (void**)tetris->levels->matrix);
    }
    free(tetris->levels);
    free(tetris->figure_sequence);
    free(tetris);
}

int solve_tetris_dfs(tetris_t* tetris, int piece_index) {
    printf("\n\n**INICIO DFS**\npiece: %c - piece_index: %d\n",
        tetris->figure_sequence[piece_index], piece_index);
    if (piece_index == tetris->depth + 1) {
        int current_height = calculate_height(tetris);
        if (current_height < tetris->min_height) {
            tetris->min_height = current_height;
            return 1;
        } else {
            return 0;
        }
    }

    int num_rotations =
        get_tetris_figure_num_rotations(tetris->figure_sequence[piece_index]);
    // printf("num_rotations: %i\n", num_rotations);
    int result = 0;

    for (int rotation = 0; rotation < num_rotations; rotation++) {
        // printf("ROTATION: %i\n", rotation);
        figure_t* figure =
            get_tetris_figure(tetris->figure_sequence[piece_index], rotation);
        // printf("FIGURE: %s\n", figure->value[0]);

        for (int num_col = 0; num_col < tetris->columns; num_col++) {
            // printf("num_col: %i\n", num_col);
            // printf("best_score: %i\n", tetris->best_score);
            if (calculate_height(tetris) > tetris->min_height) {
                continue;
            }
            // printf("despues del continue:...\n");

            if (valid_column(tetris, figure, num_col)) {
                int num_row = place_figure(tetris, figure, num_col);
                result = solve_tetris_dfs(tetris, piece_index + 1);
                if (result == 1) {
                    tetris->levels =
                        save_level(tetris->figure_sequence[piece_index],
                                   rotation, tetris->rows, tetris->columns,
                                   tetris->matrix);
                }
                remove_figure(tetris, figure, num_row, num_col);
            }
        }
    }

    return result;
}

int calculate_height(tetris_t* tetris) {
    for (int i = 0; i < tetris->rows; i++) {
        int piece_in_row = 0;

        for (int j = 0; j < tetris->columns; j++) {
            if (tetris->matrix[i][j] != 0) {
                piece_in_row = 1;
                break;
            }
        }

        if (piece_in_row == 0) {
            // printf("height1: %i\n", i);
            return i;
        }
    }

    // printf("height2: %i\n", tetris->rows);
    return tetris->rows;
}

int valid_column(tetris_t* tetris, figure_t* figure, int num_col) {
    if ((num_col + figure->width > tetris->columns) || (num_col < 0)) {
        return 0;
    } else {
        return 1;
    }
}

int place_figure(tetris_t* tetris, figure_t* figure, int num_col) {
    int num_row = 0;

    while (figure_collides(tetris, figure, num_row, num_col)) {
        num_row++;
    }

    insert_figure(tetris, figure, num_row, num_col);
    return num_row;
}

int figure_collides(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
    for (int i = 0; i < figure->height; i++) {
        for (int j = 0; j < figure->width; j++) {
            if ((tetris->matrix[num_row + i][num_col + j] != 0) &&
                (figure->value[i][j] != 0)) {
                    return 1;
            }
        }
    }

    return 0;
}

void insert_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
    for (int i = 0; i < figure->height; i++) {
        for (int j = 0; j < figure->width; j++) {
            tetris->matrix[num_row + i][num_col + j] = figure->value[i][j];
        }
    }
}

void remove_figure(tetris_t* tetris, figure_t* figure,
        int num_row, int num_col) {
    for (int i = 0; i < figure->height; i++) {
        for (int j = 0; j < figure->width; j++) {
            tetris->matrix[num_row + i][num_col + j] = 0;
        }
    }
}
