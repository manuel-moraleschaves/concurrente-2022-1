// Copyright 2022 Jose Andres Mena Arias <jose.menaarias@ucr.ac.cr>

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

#define BUFFER_SIZE 3
#define ROUNDS 2
#define PROD_MIN_TIME 0
#define PROD_MAX_TIME 100
#define CONS_MIN_TIME 0
#define CONS_MAX_TIME 750

typedef struct shared_data {
    double *buffer;

    /*
        semaphores
    */
    sem_t can_consume;
    sem_t can_produce;
} shared_data_t;

void *consume(void *data);
void *produce(void *data);
useconds_t random_between(useconds_t min, useconds_t max);

int main() {
    shared_data_t *shared_data =
                (shared_data_t *)calloc(1, sizeof(shared_data_t));

    shared_data->buffer = (double *)calloc(BUFFER_SIZE, sizeof(double));

    sem_init(&shared_data->can_consume, 0, 0);
    sem_init(&shared_data->can_produce, 0, BUFFER_SIZE);

    /*
        Create threads
    */
    pthread_t producer;
    pthread_create(&producer, NULL, produce, (void *)shared_data);

    pthread_t consumer;
    pthread_create(&consumer, NULL, consume, (void *)shared_data);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    sem_destroy(&shared_data->can_consume);
    sem_destroy(&shared_data->can_produce);

    free(shared_data->buffer);
    free(shared_data);

    return 0;
}

void *produce(void *data) {
    shared_data_t *shared_data = (shared_data_t *)data;

    size_t counter = 1;
    for (int round = 0; round < ROUNDS; ++round) {
        for (int index = 0; index < BUFFER_SIZE; ++index) {
            sem_wait(&shared_data->can_produce);
            usleep(1000 * random_between(PROD_MIN_TIME, PROD_MAX_TIME));
            shared_data->buffer[index] = counter++;
            printf("Produced %lg\n", shared_data->buffer[index]);
            sem_post(&shared_data->can_consume);
        }
    }

    return NULL;
}

void *consume(void *data) {
    shared_data_t *shared_data = (shared_data_t *)data;

    for (int round = 0; round < ROUNDS; ++round) {
        for (int index = 0; index < BUFFER_SIZE; ++index) {
            sem_wait(&shared_data->can_consume);
            usleep(1000 * random_between(CONS_MIN_TIME, CONS_MAX_TIME));
            double value = shared_data->buffer[index];
            printf("\tConsumed %lg\n", value);
            sem_post(&shared_data->can_produce);
        }
    }

    return NULL;
}

useconds_t random_between(useconds_t min, useconds_t max) {
    return min + (max > min ? (random() % (max - min)) : 0);
}
