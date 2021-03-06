// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

// Para que VS Code reconozca CLOCK_MONOTONIC
#define _POSIX_C_SOURCE 199309L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>

#include "tetris_state.h"
#include "tetris_figure_factory.h"
#include "tetris_file_ops.h"
#include "tetris_solve.h"


/**
 * @brief Rutina para validar los argumentos de entrada.
 * @details Valida el archivo inicial y la cantidad de hilos.
 * @param argc Cantidad de argumentos.
 * @param argv Argumentos enviados por consola.
 * @param file_name Nombre del archivo inicial de tetris.
 * @param thread_count Cantidad de hilos.
 * @param rank Número del proceso actual.
 * @return void.
 */
void check_params(int argc, char** argv, char file_name[100], int* thread_count,
                  int rank) {
    if (argc == 2) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            if (rank <= 0) {
                fprintf(stderr, "Error: invalid file name.\n");
            }
            exit(1);
        }
    } else if (argc == 3) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            if (rank <= 0) {
                fprintf(stderr, "Error: invalid file name.\n");
            }
            exit(2);
        }

        if (sscanf(argv[2], "%d", thread_count) != 1
            || errno) {
            if (rank <= 0) {
                fprintf(stderr, "Error: invalid thread count\n");
            }
            exit(3);
        }
    } else if (argc > 3) {
        if (rank <= 0) {
            printf("Bad parameters. Required format: ");
            printf("./tetris_solver_mpi file_name thread_count\n");
        }
        exit(4);
    }
}

/**
 * @brief Rutina para iniciar el procesamiento de la solución.
 * @details Realiza tareas entre los procesos e inicia el cálculo del tetris.
 * @param tetris Puntero al estado inicial del tetris.
 * @param thread_count Cantidad de hilos.
 * @param rank Número del proceso actual.
 * @param process_count Cantidad de procesos totales.
 * @return void.
 */
void start_process(tetris_t* tetris, int thread_count, int rank,
                   int process_count) {
    // Inicio del conteo del tiempo de ejecución
    const double startTime = MPI_Wtime();

    if (rank <= 0) {
        printf("\nResolviendo tetris...\n");
    }
    // Inicio de rutina para la solución del tetris
    solve_tetris_mpi(tetris, thread_count, rank, process_count);

    int i_got_min = 0;
    int min_height;

    // Se obtiene la altura mínima entre todos los procesos
    MPI_Allreduce(&tetris->min_height, &min_height, 1, MPI_INT,
                    MPI_MIN, MPI_COMM_WORLD);

    // En caso de que varios procesos tengan la misma altura mínima
    // (mejor puntaje), se escoge el de menor rank
    if (tetris->min_height == min_height) {
        int min_rank;
        MPI_Allreduce(&rank, &min_rank, 1, MPI_INT, MPI_MIN,
                        MPI_COMM_WORLD);

        if (rank == min_rank) {
            i_got_min = 1;
        }
    }

    // Si solo hay 1 proceso o si el proceso actual obtuvo el mejor
    // puntaje, se crean los archivos y se imprime el tiempo
    if (process_count <= 1 || i_got_min == 1) {
        // Generación de archivos finales
        generate_files(tetris, tetris->levels);

        // Cálculo del tiempo de ejecución
        double elapsed = MPI_Wtime() - startTime;
        printf("\nTiempo de ejecución: %.9lfs\n", elapsed);
    }
}

int main(int argc, char** argv) {
    // Inicializar MPI
    if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
        int rank = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        int process_count = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &process_count);

        char file_name[100] = "./test/Test1.txt";
        int const processor_count = omp_get_max_threads();
        int thread_count = processor_count;

        // Validación de parámetros
        check_params(argc, argv, file_name, &thread_count, rank);

        // Archivo inicial del tetris
        if (rank <= 0) {
            printf("File Name: %s\n", file_name);
        }
        FILE* file = fopen(file_name, "r");  // read only

        if (file) {
            // Lectura del estado inicial desde el archivo de entrada
            tetris_t* tetris = read_tetris(file, rank);

            if (!tetris) {
                if (rank <= 0) {
                    fprintf(stderr, "Error: invalid file content.\n");
                }
                return 6;
            }

            // Mensajes informativos sobre la cantidad de hilos
            int moves_count = get_tetris_figure_num_rotations(
                                tetris->figure_sequence[0]) * tetris->columns;
            if (thread_count > moves_count) {
                if (rank <= 0) {
                    printf("Threads allowed: 1-%i\n", moves_count);
                }
                thread_count = processor_count;
            }
            if (rank <= 0) {
                if (process_count > 1) {
                    printf("Number of Threads per process: %d\n", thread_count);
                } else {
                    printf("Number of Threads: %d\n", thread_count);
                }
            }

            // Iniciar el proceso
            start_process(tetris, thread_count, rank, process_count);

            // Liberación de memoria
            destroy_tetris(tetris);
            fclose(file);

        } else {
            if (rank <= 0) {
                fprintf(stderr, "Error: could not open the file %s.\n",
                        file_name);
            }
            return 7;
        }

        // Finalizar MPI
        MPI_Finalize();
    }

    return EXIT_SUCCESS;
}
