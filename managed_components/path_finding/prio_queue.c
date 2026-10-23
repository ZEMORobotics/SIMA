#include "prio_queue.h"
#include "esp_log.h"
#include "map.h"

#define TAG "prio_queue"

void pq_init(PriorityQueue *pq) {
  pq->items = (int16_t *) malloc(sizeof(int16_t));
  if (pq->items == NULL)
    ESP_LOGE(TAG, "pq_init: allocation fail");
  pq->size = 0;
}

void heapify_up(PriorityQueue *pq, uint16_t index) {
  if (index && ((pmap[pq->items[(index - 1) / 2]].f_cost > pmap[pq->items[index]].f_cost))) { 
    SWAP(&pq->items[(index - 1) / 2], &pq->items[index])
    heapify_up(pq, (index - 1) / 2);
  }
}

void enqueue(PriorityQueue *pq, int16_t n) {
  if (pq->size == MAX_SIZE) {
    ESP_LOGE(TAG, "enqueue: Queue full");
    return;
  }
  
  pq->items[pq->size++] = n;
  pq->items = (int16_t *) realloc(pq->items, (pq->size + 1) * sizeof(int16_t));
  if (pq->items == NULL)
    ESP_LOGE(TAG, "enqueue: reallocation fail");
  heapify_up(pq, pq->size - 1);
}

void heapify_down(PriorityQueue *pq, uint16_t index) {
  uint16_t smallest = index;
  uint16_t left = 2 * index + 1;
  uint16_t right = 2 * index + 2;

  if (left < pq->size
      && pmap[pq->items[left]].f_cost < pmap[pq->items[smallest]].f_cost)
    smallest = left;
  
  if (right < pq->size
      && pmap[pq->items[right]].f_cost < pmap[pq->items[smallest]].f_cost)
    smallest = right;

  if (smallest != index) {
    SWAP(&pq->items[index], &pq->items[smallest])
    heapify_down(pq, smallest);
  }
}

int16_t dequeue(PriorityQueue *pq) {
  if (!pq->size) {
    ESP_LOGE(TAG, "dequeue: Queue empty");
    abort();
  }

  int16_t item = pq->items[0];
  pq->items[0] = pq->items[--pq->size];
  pq->items = (int16_t *) realloc(pq->items, (pq->size + 1) * sizeof(int16_t));
  if (pq->items == NULL)
    ESP_LOGE(TAG, "enqueue: reallocation fail");
  heapify_down(pq, 0);
  return item;
}

int16_t peek(PriorityQueue *pq) {
  if (!pq->size) {
    ESP_LOGE(TAG, "Queue empty");
    abort();
  }
  return pq->items[0];
}

uint8_t empty(PriorityQueue *pq) {
  if (!(pq->size))
    return 1;
  else
    return 0;
}

void delete(PriorityQueue *pq) {
  free(pq->items);
  pq->items = NULL;
}