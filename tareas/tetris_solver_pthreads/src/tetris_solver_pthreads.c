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
#include "tetris_figure_factory.h"


/**
 * @brief Rutina para generar los archivos finales.
 * @details Genera los archivos finales correspondientes a cada nivel en el que se pudo colocar una pieza
 * @param tetris Puntero al estado del tetris.
 * @param base_level Puntero al nodo base de los niveles.
 * @return void.
 */
void generate_files(tetris_t* tetris, struct level_t* base_level);

void start_solver(shared_data_t* shared_data);


int main(int argc, char** argv) {
    char file_name[100] = "./test/Test1.txt";

    shared_data_t *shared_data = (shared_data_t *)
        calloc(1, sizeof(shared_data_t));

    if (!shared_data) {
        fprintf(stderr, "Error: could not create shared_data.\n");
        return 1;
    }

    shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 3) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            fprintf(stderr, "Error: invalid file name.\n");
            return 2;
        }

        if (sscanf(argv[2], "%zu", &shared_data->thread_count) != 1
            || errno) {
            fprintf(stderr, "Error: invalid thread count\n");
            return 3;
        }
    }
    printf("File Name: %s\n", file_name);
    printf("Thread Count: %zu\n", shared_data->thread_count);

    FILE* file = fopen(file_name, "r");  // read only

    if (file) {
        // Lectura del estado inicial desde el archivo de entrada
        shared_data->tetris = read_tetris(file);

        if (!shared_data->tetris) {
            fprintf(stderr, "Error: invalid file content.\n");
            return 4;
        }

        // Creación del nivel base
        // shared_data->tetris->levels = create_level('B', 0,
        //                             shared_data->tetris->rows,
        //                             shared_data->tetris->columns,
        //                             shared_data->tetris->matrix);

        // if (!shared_data->tetris->levels) {
        //    fprintf(stderr, "Error: could not create the base level.\n");
        //    return 5;
        // }

        // Inicio del conteo del tiempo de ejecución
        struct timespec start_time;
        clock_gettime(/*clk_id*/ CLOCK_MONOTONIC, &start_time);

        // Rutina recursiva para la soluciión del tetris
        printf("\nResolviendo tetris...\n");
        start_solver(shared_data);
        printf("\nTerminado todo el DFS...\n");

        // Generación de archivos finales
        generate_files(shared_data->tetris, shared_data->tetris->levels);

        // Liberación de memoria
        destroy_tetris(shared_data->tetris);
        free(shared_data);
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
        return 6;
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

void start_solver(shared_data_t* shared_data) {
    tetris_t* tetris = shared_data->tetris;

    int num_rotations =
        get_tetris_figure_num_rotations(tetris->figure_sequence[0]);

    shared_data->plays = (possible_plays_t*)
        calloc(num_rotations*tetris->columns, sizeof(possible_plays_t));

    // TODO(manum): validar error

    // Se recorren todas las posibles rotaciones de la figura
    for (int rotation = 0; rotation < num_rotations; ++rotation) {
        // Se recorren todas las columnas del tablero
        for (int num_col = 0; num_col < tetris->columns; ++num_col) {
            // TODO(manum): optimizar
            // if (calculate_height(tetris) > tetris->min_height) {
            //      continue;
            // }

            int index = (rotation * tetris->columns) + num_col;
            // printf("INDEX: %d\n", index);
            shared_data->plays[index].rotation = rotation;
            shared_data->plays[index].column = num_col;
        }
    }
    shared_data->plays_count = num_rotations * tetris->columns;
     printf("plays_count: %d\n", shared_data->plays_count);

    pthread_t *threads = (pthread_t *)calloc(shared_data->thread_count,
                                                sizeof(pthread_t));
    private_data_t *private_data = (private_data_t *)
        calloc(shared_data->thread_count, sizeof(private_data_t));

    if (threads && private_data) {
        pthread_mutex_init(&shared_data->can_access_min_height, NULL);
        pthread_mutex_init(&shared_data->can_access_levels, NULL);

        for (size_t index = 0; index < shared_data->thread_count; ++index) {
            private_data[index].thread_number = index;
            private_data[index].shared_data = shared_data;
            if (pthread_create(&threads[index], /*attr*/ NULL, solve_tetris,
                &private_data[index]) == EXIT_SUCCESS) {
            } else {
                fprintf(stderr, "error: could not create thread %zu\n", index);
                // return 6;
                break;
            }
        }

        for (size_t index = 0; index < shared_data->thread_count; ++index) {
            pthread_join(threads[index], /*value_ptr*/ NULL);
        }

        free(threads);
        free(private_data);
        pthread_mutex_destroy(&shared_data->can_access_min_height);
        pthread_mutex_destroy(&shared_data->can_access_levels);
        free(shared_data->plays);
    }
}
