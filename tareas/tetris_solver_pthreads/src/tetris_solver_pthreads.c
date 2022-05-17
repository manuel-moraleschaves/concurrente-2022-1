// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

// Para que VS Code reconozca CLOCK_MONOTONIC
#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>  // Para el cálculo del tiempo de ejecución

#include "tetris_state.h"
#include "tetris_utils.h"
#include "tetris_level.h"

/**
 * @brief Rutina para generar los archivos finales.
 * @details Genera los archivos finales correspondientes a cada nivel en el que se pudo colocar una pieza
 * @param tetris Puntero al estado del tetris.
 * @param base_level Puntero al nodo base de los niveles.
 * @return void.
 */
void generate_files(tetris_t* tetris, struct level_t* base_level);


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

        // Creación del nivel base
        struct level_t* base_level = create_level('B', 0,
                                     tetris->rows, tetris->columns,
                                     tetris->matrix);

        if (!base_level) {
            fprintf(stderr, "Error: could not create the base level.\n");
            return 4;
        }

        // Inicio del conteo del tiempo de ejecución
        struct timespec start_time;
        clock_gettime(/*clk_id*/ CLOCK_MONOTONIC, &start_time);

        // Rutina recursiva para la soluciión del tetris
        printf("\nResolviendo tetris...\n");
        solve_tetris(tetris, base_level);
        printf("\nTerminado todo el DFS...\n");

        // Generación de archivos finales
        generate_files(tetris, base_level);

        // Liberación de memoria
        destroy_levels(base_level, tetris->rows);
        printf("\nLiberado base_level...\n");
        destroy_tetris(tetris);
        fclose(file);

        // Fin del conteo del tiempo de ejecución
        struct timespec finish_time;
        clock_gettime(/*clk_id*/ CLOCK_MONOTONIC, &finish_time);

        // Cálculo del tiempo de ejecución
        double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
                         (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

        printf("\nTiempo de ejecución: %.9lfs\n", elapsed);

    } else {
        fprintf(stderr, "Error: could not open the file %s.\n", file_name);
        return 5;
    }

    return EXIT_SUCCESS;
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
