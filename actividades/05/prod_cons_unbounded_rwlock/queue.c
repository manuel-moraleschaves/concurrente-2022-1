// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// Implements a thread-safe queue

#include <assert.h>
#include <stdlib.h>

#include "queue.h"

void queue_remove_first_unsafe(queue_t* queue);
bool queue_is_empty_unsafe(const queue_t* queue);

int queue_init(queue_t* queue) {
  assert(queue);
  queue->head = NULL;
  queue->tail = NULL;
  return pthread_rwlock_init(&queue->rwlock, NULL);
}

int queue_destroy(queue_t* queue) {
  queue_clear(queue);
  return pthread_rwlock_destroy(&queue->rwlock);
}

bool queue_is_empty_unsafe(const queue_t* queue) {
  assert(queue);
  return queue->head == NULL;
}

bool queue_is_empty(queue_t* queue) {
  assert(queue);
  pthread_rwlock_rdlock(&queue->rwlock);
  bool result = queue_is_empty_unsafe(queue);
  pthread_rwlock_unlock(&queue->rwlock);
  return result;
}

int queue_enqueue(queue_t* queue, const size_t data) {
  assert(queue);
  int error = EXIT_SUCCESS;

  queue_node_t* new_node = (queue_node_t*)
    calloc(1, sizeof(queue_node_t));

  if (new_node) {
    new_node->data = data;

    pthread_rwlock_wrlock(&queue->rwlock);
    if (queue->tail) {
      queue->tail = queue->tail->next = new_node;
    } else {
      queue->head = queue->tail = new_node;
    }
    pthread_rwlock_unlock(&queue->rwlock);
  } else {
    error = EXIT_FAILURE;
  }

  return error;
}

int queue_dequeue(queue_t* queue, size_t* data) {
  assert(queue);
  int error = 0;

  pthread_rwlock_wrlock(&queue->rwlock);
    if (!queue_is_empty_unsafe(queue)) {
      if (data) {
        *data = queue->head->data;
      }
      queue_remove_first_unsafe(queue);
    } else {
      error = EXIT_FAILURE;
    }
  pthread_rwlock_unlock(&queue->rwlock);

  return error;
}

void queue_remove_first_unsafe(queue_t* queue) {
  assert(queue);
  assert(!queue_is_empty_unsafe(queue));
  queue_node_t* node = queue->head;
  queue->head = queue->head->next;
  free(node);
  if (queue->head == NULL) {
    queue->tail = NULL;
  }
}

void queue_clear(queue_t* queue) {
  assert(queue);
  pthread_rwlock_wrlock(&queue->rwlock);
    while (!queue_is_empty_unsafe(queue)) {
      queue_remove_first_unsafe(queue);
    }
  pthread_rwlock_unlock(&queue->rwlock);
}
