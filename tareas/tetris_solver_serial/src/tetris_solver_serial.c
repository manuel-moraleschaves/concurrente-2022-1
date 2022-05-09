// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tetris_state.h"
#include "tetris_utils.h"


int main(int argc, char** argv) {
    char file_name[100] = "./test/Test1.txt";

    if (argc == 2) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            fprintf(stderr, "Error: invalid file name.\n");
            return 1;
        }
    }

    printf("File Name: %s\n", file_name);
    FILE* file = fopen(file_name, "r");  // read only

    if (file) {
        // Lectura del estado inicial desde el archivo de entrada
        tetris_t* tetris = read_tetris(file);

        if (!tetris) {
            fprintf(stderr, "Error: invalid file content.\n");
            return 3;
        }

        // Para generar un archivo de control
        FILE* file2 = fopen("Out.txt", "w");

        // Rutina recursiva para la soluciión del tetris
        solve_tetris_dfs(tetris, 0, file2);

        // Generación de archivos finales
        char output_name[50];
        FILE* out_file;
        for (int i = 0; i <= tetris->depth; ++i) {
            sprintf(output_name, "tetris_play_%i.txt", i);
            out_file = fopen(output_name, "w");
            fprintf(out_file, "%zu\n", tetris->id);
            fprintf(out_file, "%c\n", tetris->figure_sequence[i]);
            fprintf(out_file, "%i\n", tetris->levels[i].shape);
            fprintf(out_file, "%i\n", tetris->rows);
            fprintf(out_file, "%i\n", tetris->columns);
            print_matrix_file(tetris->rows,
                tetris->levels[i].matrix, out_file);
        }

        // Liberación de memoria
        destroy_tetris(tetris);
        fclose(file);
        fclose(out_file);
        fclose(file2);

    } else {
        fprintf(stderr, "Error: could not open the file %s.\n", file_name);
        return 4;
    }

    return EXIT_SUCCESS;
}
