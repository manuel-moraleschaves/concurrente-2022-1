// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

int create_threads(size_t thread_count);
void* run(void*);

typedef struct shared_data {
    size_t thread_count;
    char message[6];
    sem_t can_print_hello;
    sem_t can_print_world;
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

        if (private_data && shared_data) {
            sem_init(&shared_data->can_print_hello, /*pshared*/ 0, 1);
            sem_init(&shared_data->can_print_world, /*pshared*/ 0, 0);

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

            sem_destroy(&shared_data->can_print_hello);
            sem_destroy(&shared_data->can_print_world);
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
    shared_data_t* shared_data = data->shared_data;
    
    if (data->thread_num % 2 == 0) {
        sem_wait(&shared_data->can_print_hello);
        sscanf("hello", "%s", data->message);
        printf("%zu: %s!\n", data->thread_num, data->message);
        sem_post(&shared_data->can_print_world);
        
    } else {
        sem_wait(&shared_data->can_print_world);
        sscanf("world", "%s", data->message);
        printf("%zu: %s!\n", data->thread_num, data->message);
        sem_post(&shared_data->can_print_hello);
    }
    
    return NULL;
}
