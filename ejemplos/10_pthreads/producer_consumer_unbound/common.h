// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a bounded buffer

#ifndef COMMON_H
#define COMMON_H

// @see `man feature_test_macros`
// #define _DEFAULT_SOURCE

#include <semaphore.h>
#include <unistd.h>

#include "queue.h"

typedef struct simulation {
  size_t unit_count;
  queue_t queue;
  size_t producer_count;
  size_t consumer_count;
  sem_t can_consume;
  sem_t can_access_next_unit;
  size_t next_unit;
  sem_t can_access_consumed_count;
  size_t consumed_count;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;
} simulation_t;

useconds_t random_between(useconds_t min, useconds_t max);

#endif  // COMMON_H
