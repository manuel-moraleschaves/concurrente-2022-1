// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a bounded buffer

// @see `man feature_test_macros`
#define _DEFAULT_SOURCE

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"
#include "producer.h"
#include "simulation.h"


int analyze_arguments(simulation_t* simulation, int argc, char* argv[]);
int create_consumers_producers(simulation_t* simulation);


simulation_t* simulation_create() {
  simulation_t* simulation = (simulation_t*)
    calloc(1, sizeof(simulation_t));
  if (simulation) {
    simulation->unit_count = 0;
    queue_init(&simulation->queue);
    sem_init(&simulation->can_consume, 0, 0);
    // todo: change for mutex
    sem_init(&simulation->can_access_next_unit, 0, 1);
    sem_init(&simulation->can_access_consumed_count, 0, 1);
  }
  return simulation;
}

void simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  sem_destroy(&simulation->can_access_next_unit);
  sem_destroy(&simulation->can_access_consumed_count);
  free(simulation);
}

int simulation_run(simulation_t* simulation, int argc, char* argv[]) {
  int error = analyze_arguments(simulation, argc, argv);
  if (error == EXIT_SUCCESS) {
//    error = queue_init(&simulation->queue);
    if (error == EXIT_SUCCESS) {
      // sem_init(&simulation->can_consume, 0, 0);

      unsigned int seed = 0;
      getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
      srandom(seed);

      struct timespec start_time;
      clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

      error = create_consumers_producers(simulation);

      struct timespec finish_time;
      clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

      double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
      printf("execution time: %.9lfs\n", elapsed);

      sem_destroy(&simulation->can_consume);
      queue_destroy(&simulation->queue);
    } else {
      fprintf(stderr, "error: could not create buffer\n");
      error = 1;
    }
  }
  return error;
}

int analyze_arguments(simulation_t* simulation, int argc, char* argv[]) {
  int error = 0;
  if (argc == 8) {
    if (sscanf(argv[1], "%zu", &simulation->unit_count) != 1
      || simulation->unit_count == 0) {
        fprintf(stderr, "error: invalid unit count\n");
        error = 2;
    } else if (sscanf(argv[2], "%zu", &simulation->producer_count) != 1
      || simulation->producer_count == 0) {
        fprintf(stderr, "error: invalid producer count\n");
        error = 3;
    } else if (sscanf(argv[3], "%zu", &simulation->consumer_count) != 1
      || simulation->consumer_count == 0) {
        fprintf(stderr, "error: invalid consumer count\n");
        error = 4;
    } else if (sscanf(argv[4], "%u", &simulation->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = 5;
    } else if (sscanf(argv[5], "%u", &simulation->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = 6;
    } else if (sscanf(argv[6], "%u", &simulation->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = 7;
    } else if (sscanf(argv[7], "%u", &simulation->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = 8;
    }
  } else {
    fprintf(stderr, "usage: producer_consumer unit_count producer_count"
      " consumer_count producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = 1;
  }
  return error;
}

pthread_t* create_threads(size_t count, void*(*subroutine)(void*), void* data) {
  pthread_t* threads = (pthread_t*) calloc(count, sizeof(pthread_t));
  if (threads) {
    for (size_t index = 0; index < count; ++index) {
      if (pthread_create(&threads[index], /*attr*/ NULL, subroutine, data)
         == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        // todo: destroy created threads and return NULL
        break;
      }
    }
  }
  return threads;
}

int wait_threads(size_t count, pthread_t* threads) {
  int error = EXIT_SUCCESS;
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    error += pthread_join(threads[index], /*value_ptr*/ NULL);
  }
  free(threads);
  return error;
}

int create_consumers_producers(simulation_t* simulation) {
  assert(simulation);
  int error = EXIT_SUCCESS;

  pthread_t* producers = create_threads(simulation->producer_count, produce
    , simulation);
  pthread_t* consumers = create_threads(simulation->consumer_count, consume
    , simulation);

  if (producers && consumers) {
    wait_threads(simulation->producer_count, producers);
    for (size_t index = 0; index < simulation->consumer_count; ++index) {
      sem_post(&simulation->can_consume);
    }
    wait_threads(simulation->consumer_count, consumers);
  } else {
    fprintf(stderr, "error: could not allocate create threads\n");
    error = 22;
  }

  return error;
}
