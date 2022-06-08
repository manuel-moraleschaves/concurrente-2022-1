// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int create_threads(size_t thread_count);
void* run(void*);

typedef struct shared_data {
    size_t position;
    size_t thread_count;
    pthread_mutex_t can_access_position;
    pthread_barrier_t barrier;
} shared_data_t;

typedef struct private_data {
    size_t thread_num;
    shared_data_t* shared_data;
} private_data_t;

int main(int argc, char** arg) {
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
    pthread_t* threads = calloc(thread_count, sizeof(pthread_t));

    if (threads) {
        private_data_t* private_data = (private_data_t*)malloc(
                                    thread_count * sizeof(private_data_t));

        shared_data_t* shared_data =
                (shared_data_t*)calloc(1, sizeof(shared_data_t));

        if (private_data && shared_data) {
            shared_data->thread_count = thread_count;
            shared_data->position = 0;
            pthread_mutex_init(&shared_data->can_access_position, NULL);
            pthread_barrier_init(&shared_data->barrier, NULL, thread_count);

            for (size_t i = 0; i < thread_count; ++i) {
                private_data[i].thread_num = i;
                private_data[i].shared_data = shared_data;
                if (pthread_create(&threads[i], NULL, run,
                    (void*)&private_data[i]) != EXIT_SUCCESS) {
                    fprintf(stderr, "Could not create thread %zu.\n", i);
                    return EXIT_FAILURE;
                }
            }

            for (size_t i = 0; i < thread_count; ++i) {
                pthread_join(threads[i], NULL);
            }

            free(private_data);
            pthread_mutex_destroy(&shared_data->can_access_position);
            pthread_barrier_destroy(&shared_data->barrier);
            free(shared_data);

        } else {
            fprintf(stderr, "Unable to allocate memory for %zu threads\n",
                thread_count);
            return EXIT_FAILURE;
        }        free(threads);

    } else {
        fprintf(stderr, "Unable to allocate memory for %zu threads\n",
            thread_count);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void* run(void* params) {
    private_data_t* data = (private_data_t*)params;
    shared_data_t* shared_data = data->shared_data;

    printf("Thread %zu/%zu: is waiting...\n", data->thread_num,
            data->shared_data->thread_count);
    pthread_barrier_wait(&shared_data->barrier);

    pthread_mutex_lock(&shared_data->can_access_position);
    data->shared_data->position++;
    printf("Thread %zu/%zu: Position %zu\n", data->thread_num,
            data->shared_data->thread_count, data->shared_data->position);
    pthread_mutex_unlock(&shared_data->can_access_position);

    return NULL;
}
