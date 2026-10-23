#ifndef PRIO_QUEUE_H
#define PRIO_QUEUE_H

#include <stdlib.h>

#define MAX_SIZE 1200
#define SWAP(A, B) do { \
              int16_t temp = *A; \
              *A = *B; \
              *B = temp; \
            } while (0);

typedef struct prio_queue {
  int16_t *items;
  uint16_t size;
} PriorityQueue;

void pq_init(PriorityQueue *pq);
void heapify_up(PriorityQueue *pq, uint16_t index);
void enqueue(PriorityQueue *pq, int16_t n);
void heapify_down(PriorityQueue *pq, uint16_t index);
int16_t dequeue(PriorityQueue *pq);
int16_t peek(PriorityQueue *pq);
uint8_t empty(PriorityQueue *pq);
void delete(PriorityQueue *pq);

#endif // PRIO_QUEUE_H

