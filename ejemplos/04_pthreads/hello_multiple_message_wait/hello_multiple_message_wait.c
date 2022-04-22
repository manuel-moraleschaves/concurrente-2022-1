// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int create_threads(size_t thread_count);
void* run(void*);

typedef struct shared_data {
    size_t thread_count;
    size_t next;
    char message[6];
} shared_data_t;

typedef struct private_data {
    size_t thread_num;
    shared_data_t* shared_data;
    char message[6];
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

        shared_data_t* shared_data = (shared_data_t*)calloc(1,
                                        sizeof(shared_data_t));

        shared_data->next = 0;

        if (private_data && shared_data) {
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

            printf("Hello from the main thread. Shared message: %s\n",
                    shared_data->message);

            free(private_data);
            free(shared_data);

        } else {
            fprintf(stderr, "Unable to allocate memory for %zu threads\n",
                thread_count);
            return EXIT_FAILURE;
        }

        free(threads);

    } else {
        fprintf(stderr, "Unable to allocate memory for %zu threads\n",
            thread_count);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void* run(void* params) {
    private_data_t* data = (private_data_t*)params;

    // busy wait - espera activa
    while (data->shared_data->next != data->thread_num) {}

    if (data->thread_num % 2 == 0) {
        sscanf("hello", "%s", data->message);
        sscanf("hello", "%s", data->shared_data->message);
    } else {
        sscanf("world", "%s", data->message);
        sscanf("world", "%s", data->shared_data->message);
    }
    printf("%zu: %s!\n", data->thread_num, data->message);

    data->shared_data->next = data->thread_num + 1;

    return NULL;
}
