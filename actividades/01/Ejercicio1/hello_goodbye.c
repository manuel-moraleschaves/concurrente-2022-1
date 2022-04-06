#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* run1(void*);
void* run2(void*);

int main() {

    size_t value = sysconf(_SC_NPROCESSORS_ONLN);    
    
    pthread_t thread1;
    pthread_t thread2;

    if ((pthread_create(&thread1, NULL /*attr*/, run1, (void*)value) == EXIT_SUCCESS) && (pthread_create(&thread2, NULL /*attr*/, run2, (void*)value) == EXIT_SUCCESS)) {
        printf("Hello and goodbye world! %zu\n", value);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void* run1(void* param) {    
    size_t value = (size_t)param;
    printf("Hello world!: %zu\n", value);

    return NULL;
}

void* run2(void* param) {    
    size_t value = (size_t)param;
     printf("Goodbye world!: %zu\n", value);

    return NULL;
}