// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

// Para que VS Code reconozca CLOCK_MONOTONIC
#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>

#include "tetris_state.h"
#include "tetris_figure_factory.h"
#include "tetris_file_ops.h"
#include "tetris_solve.h"


int main(int argc, char** argv) {
    char file_name[100] = "./test/Test1.txt";
    int const processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    int thread_count = processor_count;

    if (argc == 2) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            fprintf(stderr, "Error: invalid file name.\n");
            return 1;
        }
    } else if (argc == 3) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            fprintf(stderr, "Error: invalid file name.\n");
            return 2;
        }

        if (sscanf(argv[2], "%d", &thread_count) != 1
            || errno) {
            fprintf(stderr, "Error: invalid thread count\n");
            return 3;
        }
    } else if (argc > 3) {
        printf("Bad parameters. Required format: ");
        printf("tetris_solver_omp file_name thread_count\n");
        return 4;
    }

    printf("File Name: %s\n", file_name);
    FILE* file = fopen(file_name, "r");  // read only

    if (file) {
        // Lectura del estado inicial desde el archivo de entrada
        tetris_t* tetris = read_tetris(file);

        if (!tetris) {
            fprintf(stderr, "Error: invalid file content.\n");
            return 6;
        }

        fprintf(stderr, "\nNumber of threads suggested: %i", processor_count*2);
        int moves_count = get_tetris_figure_num_rotations(
                                tetris->figure_sequence[0]) * tetris->columns;
        if (thread_count > moves_count) {
            fprintf(stderr, "\nNumber of threads allowed: 1-%i", moves_count);
            thread_count = processor_count;
        }

        printf("\nNumber of threads to be created: %d\n", thread_count);

        // Inicio del conteo del tiempo de ejecución
        double start_time = omp_get_wtime();

        // Inicio de rutina para la soluciión del tetris
        printf("\nResolviendo tetris...\n");
        solve_tetris(tetris, thread_count);
        printf("Proceso finalizado...\n");

        // Generación de archivos finales
        generate_files(tetris, tetris->levels);

        // Liberación de memoria
        destroy_tetris(tetris);
        fclose(file);

        // Fin del conteo del tiempo de ejecución
        double finish_time = omp_get_wtime();

        // Cálculo del tiempo de ejecución
        double elapsed = finish_time - start_time;

        printf("\nTiempo de ejecución: %.9lfs\n", elapsed);

    } else {
        fprintf(stderr, "Error: could not open the file %s.\n", file_name);
        return 7;
    }

    return EXIT_SUCCESS;
}
