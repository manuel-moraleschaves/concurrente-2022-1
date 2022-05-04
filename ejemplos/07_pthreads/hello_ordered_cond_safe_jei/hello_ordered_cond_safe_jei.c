// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr>
// All threads greet in their rank order by using an array of semaphores
// After greeting a thread turns on the next thread's semaphore

#include <assert.h> 
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GREET_LEN 50

typedef struct {
    size_t thread_count;
    size_t position;
    char** greets;
} shared_data_t;

typedef struct {
    size_t thread_number;
    shared_data_t* shared_data;
} private_data_t;

int measure_greet(shared_data_t* shared_data);
int create_threads(shared_data_t* shared_data);
void* run(void* data);
void** create_matrix(size_t row_count, size_t col_count, size_t element_size);
void free_matrix(const size_t row_count, void** matrix);

int main(int argc, char* argv[]) {
    int error = EXIT_SUCCESS;

    shared_data_t* shared_data = (shared_data_t*)
      calloc(1, sizeof(shared_data_t));
    
    if (shared_data) {
        shared_data->position = 0;
        shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
        if (argc == 2) {
            if (sscanf(argv[1], "%zu", &shared_data->thread_count) != 1 || errno) {
                fprintf(stderr, "error: invalid thread count\n");
                error = 1;
            }
        }

        if (error == EXIT_SUCCESS) {
            shared_data->greets = (char**) create_matrix(shared_data->thread_count
                , GREET_LEN, sizeof(char));
            
            if (shared_data->greets) {
                error = measure_greet(shared_data);
                free_matrix(shared_data->thread_count, (void**)shared_data->greets);
            } else {
                fprintf(stderr, "error: could not allocate semaphores\n");
                error = 3;
            }

            free(shared_data);
        } else {
            fprintf(stderr, "error: could not allocate shared memory\n");
            error = 2;
        }
    }

    return error;
}

int measure_greet(shared_data_t* shared_data) {
    struct timespec start_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

    int error = create_threads(shared_data);

    struct timespec finish_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

    for (size_t index = 0; index < shared_data->thread_count; ++index) {
        printf("%s\n", shared_data->greets[index]);
    }
    
    double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("execution time: %.9lfs\n", elapsed);
    return error;
}

int create_threads(shared_data_t* shared_data) {
    assert(shared_data);
    int error = EXIT_SUCCESS;
    pthread_t* threads = (pthread_t*) calloc(shared_data->thread_count
        , sizeof(pthread_t));
    private_data_t* private_data = (private_data_t*)
        calloc(shared_data->thread_count, sizeof(private_data_t));
    if (threads && private_data) {
        for (size_t index = 0; index < shared_data->thread_count; ++index) {
            private_data[index].thread_number = index;
            private_data[index].shared_data = shared_data;

            if (error == EXIT_SUCCESS) {
                if (pthread_create(&threads[index], /*attr*/ NULL, run,
                    &private_data[index]) == EXIT_SUCCESS) {
                } else {
                    fprintf(stderr, "error: could not create thread %zu\n", index);
                    error = 21;
                    shared_data->thread_count = index;
                    break;
                }
            } else {
                fprintf(stderr, "error: could not init semaphore %zu\n", index);
                error = 22;
                shared_data->thread_count = index;
                break;
            }      
        }

        printf("Hello from main thread\n");

        for (size_t index = 0; index < shared_data->thread_count; ++index) {
            pthread_join(threads[index], /*value_ptr*/ NULL);
        }
        free(threads);
        free(private_data);
    } else {
        fprintf(stderr, "error: could not allocate memory for %zu threads\n",
            shared_data->thread_count);
        error =  22;
    }

    return error;
}

void* run(void* data) {
    const private_data_t* private_data = (private_data_t*)data;
    shared_data_t* shared_data = private_data->shared_data;
    const size_t my_thread_id = private_data->thread_number;
    const size_t thread_count = shared_data->thread_count;
    
    // Do heavy task

    sprintf(shared_data->greets[my_thread_id], "Hello from thread %zu of %zu", 
        my_thread_id, thread_count);
     
    return NULL;
}

void** create_matrix(size_t row_count, size_t col_count, size_t element_size) {
    void** matrix = (void**) calloc(row_count, sizeof(void*));
    if (matrix == NULL) {
        return NULL;
    }

    for (size_t row = 0; row < row_count; ++row) {
        if ((matrix[row] = calloc(col_count, element_size)) == NULL) {
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
