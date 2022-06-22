// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include "tetris_file_ops.h"
#include "tetris_utils.h"

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

void generate_files(tetris_t* tetris, struct level_t* base_level) {
    int i = 0;
    struct level_t* current = tetris->levels;
    char output_name[50];

    while (current != NULL) {
        printf("\nLETRA %c\n", current->figure);
        print_matrix(tetris->rows, current->matrix);

        if (i > 0) {
            snprintf(output_name, sizeof(output_name),
                "tetris_play_%i.txt", i-1);
            FILE* out_file = fopen(output_name, "w");
            fprintf(out_file, "%zu\n", tetris->id);
            fprintf(out_file, "%c\n", current->figure);
            fprintf(out_file, "%i\n", current->rotation);
            fprintf(out_file, "%i\n", tetris->rows);
            fprintf(out_file, "%i\n", tetris->columns);
            print_matrix_file(tetris->rows, current->matrix, out_file);
            fclose(out_file);
        }

        current = current->next;
        i++;
    }

    /* En caso de que del todo no se haya podido colocar una pieza, provoca
    *  que no se almacenaran los niveles de mejor puntaje en el estado de tetris
    *  por lo que se recorre este nodo auxiliar */       
    if (i < tetris->depth) {
        current = base_level;

        // Llegar al nodo correspondiente
        for (int j = 0; j < i; ++j) {
            current = current->next;
        }

        while (current != NULL) {
            printf("\nLETRA %c\n", current->figure);
            print_matrix(tetris->rows, current->matrix);

            snprintf(output_name, sizeof(output_name),
                "tetris_play_%i.txt", i-1);
            FILE* out_file = fopen(output_name, "w");
            fprintf(out_file, "%zu\n", tetris->id);
            fprintf(out_file, "%c\n", current->figure);
            fprintf(out_file, "%i\n", current->rotation);
            fprintf(out_file, "%i\n", tetris->rows);
            fprintf(out_file, "%i\n", tetris->columns);
            print_matrix_file(tetris->rows, current->matrix, out_file);
            fclose(out_file);

            current = current->next;
            i++;
        }
    }
}
