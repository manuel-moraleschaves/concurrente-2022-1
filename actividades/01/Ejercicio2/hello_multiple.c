#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* run(void*);

int main() {

    pthread_t thread;

    for (size_t i = 0; i < 4; ++i) {
        if (pthread_create(&thread, NULL, run, (void*)i) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    }
    printf("Hello from the main thread\n");
    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}

void* run(void* params) {
    printf("Hello from the secondary thread %zu!\n", (size_t)params);
    return NULL;
}
