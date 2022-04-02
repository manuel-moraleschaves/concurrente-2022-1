#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* run(void*);

int main() {

    pthread_t thread;

    if (pthread_create(&thread, NULL /*attr*/, run, (void*)2022) == EXIT_SUCCESS) {
        printf("Hello word from main thread!\n");
        pthread_join(thread, NULL);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void* run(void* param) {

    printf("Hello word from secondary thread!\n");
    printf("param: %p\n", param);

    return NULL;
}