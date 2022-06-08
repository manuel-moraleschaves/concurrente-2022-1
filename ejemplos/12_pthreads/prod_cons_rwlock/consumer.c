// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a unbounded buffer

#include <stdio.h>

#include "common.h"
#include "consumer.h"

#include <stdbool.h>
#include <stdio.h>

void* consume(void* data) {
  simulation_t* simulation = (simulation_t*)data;

  while (true) {
    sem_wait(&simulation->can_access_consumed_count);
    if (simulation->consumed_count >= simulation->unit_count) {
      sem_post(&simulation->can_access_consumed_count);
      break;
    }
    ++simulation->consumed_count;
    sem_post(&simulation->can_access_consumed_count);

    sem_wait(&simulation->can_consume);
    size_t value = 0;
    queue_dequeue(&simulation->queue, &value);
    printf("\tConsumed %zu\n", value);
    usleep(1000 * random_between(simulation->consumer_min_delay
      , simulation->consumer_max_delay));
  }

  return NULL;
}
