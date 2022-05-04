// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GREET_SIZE 100

int create_threads(size_t thread_count);
void *run(void *);
void** create_matrix(size_t row_count, size_t col_count, size_t element_size);
void free_matrix(const size_t row_count, void** matrix);

typedef struct shared_data {
    size_t thread_count;
    char** greets;
} shared_data_t;

typedef struct private_data {
    size_t thread_num;
    shared_data_t *shared_data;
} private_data_t;

int main(int argc, char **arg) {
    size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);

    if (argc == 2) {
        if (sscanf(arg[1], "%zu", &thread_count) != 1 || errno) {
            fprintf(stderr, "Invalid number of threads.\n");
            return EXIT_FAILURE;
        }
    }

    int error = create_threads(thread_count);

    return error;
}

int create_threads(size_t thread_count) {
    pthread_t *threads = calloc(thread_count, sizeof(pthread_t));

    if (threads) {
        private_data_t *private_data = (private_data_t *)malloc(
            thread_count * sizeof(private_data_t));

        if (!private_data) {
            fprintf(stderr, "Could not allocate memmory for private_data\n");
            free(threads);

            return EXIT_FAILURE;
        }

        shared_data_t *shared_data =
            (shared_data_t *)calloc(1, sizeof(shared_data_t));

        if (!shared_data) {
            fprintf(stderr, "Could not allocate memmory for shared data\n");
            free(threads);
            free(private_data);

            return EXIT_FAILURE;
        }
        shared_data->thread_count = thread_count;
        shared_data->greets = (char**)create_matrix(thread_count,
            GREET_SIZE, sizeof(char));

        if (!shared_data->greets) {
            fprintf(stderr, "Could not create matrix\n");
            free(threads);
            free(private_data);
            free(shared_data);

            return EXIT_FAILURE;
        }

        for (size_t i = 0; i < thread_count; ++i) {
            private_data[i].thread_num = i;
            private_data[i].shared_data = shared_data;
            if (pthread_create(&threads[i], NULL, run,
                               (void *)&private_data[i]) != EXIT_SUCCESS) {
                fprintf(stderr, "Could not create thread %zu.\n", i);
            }
        }
        for (size_t i = 0; i < thread_count; ++i) {
            pthread_join(threads[i], NULL);
        }

        for (size_t i = 0; i < thread_count; ++i) {
            printf("%s\n", shared_data->greets[i]);
        }

        printf("Hello from the main thread\n");

        free_matrix(thread_count, (void**)shared_data->greets);
        free(shared_data);
        free(private_data);
        free(threads);
    } else {
        fprintf(stderr, "Unable to allocate memory for %zu threads\n",
                thread_count);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void *run(void *params) {
    private_data_t *data = (private_data_t *)params;
    shared_data_t *shared_data = (shared_data_t *)data->shared_data;
    size_t thread_num = data->thread_num;
    snprintf(shared_data->greets[thread_num], GREET_SIZE,
        "Thread %zu/%zu: Hello!", thread_num, shared_data->thread_count);

    return NULL;
}

void** create_matrix(size_t row_count, size_t col_count, size_t element_size) {
  void** matrix = (void**) calloc(row_count, sizeof(void*));
  if ( matrix == NULL ) {
    return NULL;
  }

  for (size_t row = 0; row < row_count; ++row) {
    if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
      free_matrix(row_count, matrix);
      return NULL;
    }
  }

  return matrix;
}

void free_matrix(const size_t row_count, void** matrix) {
  if (matrix) {
    for (size_t row = 0; row < row_count; ++row) {
      free(matrix[row]);
    }
  }

  free(matrix);
}
