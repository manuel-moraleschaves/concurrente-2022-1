// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_threads(size_t thread_count);
void *run(void *);

typedef struct shared_data {
    size_t thread_count;
    sem_t *can_greet;
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
        shared_data->can_greet = (sem_t *)calloc(thread_count, sizeof(sem_t));

        if (!shared_data->can_greet) {
            fprintf(stderr, "Could not allocate memmory for semaphores\n");
            free(threads);
            free(private_data);
            free(shared_data);

            return EXIT_FAILURE;
        }

        for (size_t i = 0; i < thread_count; ++i) {
            private_data[i].thread_num = i;
            sem_init(&shared_data->can_greet[i], 0, !i);
            private_data[i].shared_data = shared_data;
            if (pthread_create(&threads[i], NULL, run,
                               (void *)&private_data[i]) != EXIT_SUCCESS) {
                fprintf(stderr, "Could not create thread %zu.\n", i);
            }
        }
        for (size_t i = 0; i < thread_count; ++i) {
            pthread_join(threads[i], NULL);
        }
        printf("Hello from the main thread\n");

        for (size_t i = 0; i < thread_count; ++i) {
            sem_destroy(&shared_data->can_greet[i]);
        }
        free(shared_data->can_greet);
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

    sem_wait(&shared_data->can_greet[thread_num]);

    printf("Thread %zu/%zu: Hello!\n", data->thread_num,
           data->shared_data->thread_count);

    size_t next_index = (thread_num + 1) % shared_data->thread_count;
    sem_post(&shared_data->can_greet[next_index]);

    return NULL;
}
