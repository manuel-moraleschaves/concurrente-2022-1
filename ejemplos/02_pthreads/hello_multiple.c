#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* run(void*);

int main(int argc, char** arg) {

    size_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    
    if (argc > 1) {
        sscanf(arg[1], "%zu", &thread_count);
    }

    pthread_t thread[thread_count];

    for (size_t i = 0; i < thread_count; ++i) {
        if (pthread_create(&thread[i], NULL, run, (void*)i) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }

    printf("Hello from the main thread\n");

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(thread[i], NULL);
    }
    
    return EXIT_SUCCESS;
}

void* run(void* params) {
    printf("Hello from the secondary thread %zu!\n", (size_t)params);
    return NULL;
}