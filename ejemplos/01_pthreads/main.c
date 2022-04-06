#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* run(void*);

int main() {

    pthread_t thread;

    if (pthread_create(&thread, NULL /*attr*/, run, (void*)2022) == EXIT_SUCCESS) {
        printf("Hello world from main thread!\n");
        pthread_join(thread, NULL);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void* run(void* param) {    
    size_t value = (size_t)param; // siempre usar size_t en lugar de int
    printf("Hello world from secondary thread!\n");
    printf("param: %zu\n", value); // zu para size_t

    return NULL;
}