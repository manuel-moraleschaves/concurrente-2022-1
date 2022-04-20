// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int create_threads(size_t thread_count);
void* run(void*);

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
    pthread_t* threads = (pthread_t*)calloc(thread_count, sizeof(pthread_t));

    if (threads) {
        for (size_t i = 0; i < thread_count; ++i) {
            if (pthread_create(&threads[i], NULL, run, (void*)i)
                    != EXIT_SUCCESS) {
                fprintf(stderr, "Could not create thread %zu.\n", i);
                return EXIT_FAILURE;
            }
        }
        printf("Hello from the main thread\n");

        for (size_t i = 0; i < thread_count; ++i) {
            pthread_join(threads[i], NULL);
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
    printf("Hello from the secondary thread %zu!\n", (size_t)params);
    return NULL;
}
