#ifndef PRIO_QUEUE_H
#define PRIO_QUEUE_H

#include <stdlib.h>
#include "map.h"

#define MAX_SIZE 1200
#define SWAP(A, B) do { \
              Node temp = *A; \
              *A = *B; \
              *B = temp; \
            } while (0);

typedef struct prio_queue {
  Node *items;
  uint16_t size;
} PriorityQueue;

void pq_init(PriorityQueue *pq);
void heapify_up(PriorityQueue *pq, uint16_t index);
void enqueue(PriorityQueue *pq, Node const *n);
void heapify_down(PriorityQueue *pq, uint16_t index);
Node dequeue(PriorityQueue *pq);
Node peek(PriorityQueue *pq);
uint8_t empty(PriorityQueue *pq);
void delete(PriorityQueue *pq);

#endif // PRIO_QUEUEU_H

