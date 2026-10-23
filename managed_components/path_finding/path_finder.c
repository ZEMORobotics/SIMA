#include <stdio.h>
#include <math.h>
#include "map.h"
#include "path_finder.h"
#include "prio_queue.h"
#include "esp_log.h"

#define MIN(a, b) ((a < b) ? a : b)

#define TAG_PF "path_find"

static int16_t init_node = 0, end_node = 0;

void set_init_num(uint16_t x, uint16_t y) {
  init_node = XY_TO_NODE(x, y);
}

void set_end_num(uint16_t x, uint16_t y) {
  end_node = XY_TO_NODE(x, y);
}

void set_init_node(void) {
  pmap[init_node].prev = NAB;
  pmap[init_node].g_cost = 0;
  pmap[init_node].f_cost = heuristic(init_node);
  pmap[init_node].state = NONE;
}

void set_end_node(void) { 
  pmap[end_node].prev = NAB;
  pmap[end_node].g_cost = NCB;
  pmap[end_node].f_cost = NCB;
  pmap[end_node].state = NONE;
}

// Obavezno free da se uradi nakon provere
int16_t *get_neighbors(int16_t node_number, int8_t *len) {
  int16_t *neighbors = (int16_t *) malloc(8 * sizeof(int16_t));

  if (neighbors == NULL)
    ESP_LOGE(TAG_PF, "get_neighbors: allocation fail");

  uint8_t node_row = node_number / ROW_SIZE;
  uint8_t node_col = node_number % ROW_SIZE;

  int8_t dy[3] = {-1, 0, 1}, dx[3] = {-1, 0, 1};
  uint8_t k = 0;
  for (uint8_t j = 0; j < 3; ++j) {
    for (uint8_t i = 0; i < 3; ++i) {
      if (j == 1 && i == 1)
        continue;
      else {
        if ((node_row + dy[j] < 0) || (node_row + dy[j] >= NUMBER_OF_ROWS) ||
            (node_col + dx[i] < 0) || (node_col + dx[i] >= ROW_SIZE))
            continue;

        neighbors[k++] = (node_row + dy[j]) * ROW_SIZE + (node_col + dx[i]);
      }
    }
  }

  if (k < 8) {
    neighbors = (int16_t *) realloc(neighbors, (k + 1) * sizeof(int16_t));

    if (neighbors == NULL)
      ESP_LOGE(TAG_PF, "get_neighbors: reallocation fail");
  }

  *len = k;

  return neighbors;
}

int8_t is_obstacle(int16_t node_number) {
  if (pmap[node_number].element == QUARRY || pmap[node_number].element == CASTLE ||
      pmap[node_number].element == ROBOT)
    return 1;
  else
    return 0;
}

uint16_t heuristic(int16_t node_number) {
  register uint16_t dx, dy;
  register uint16_t xy_node[2], xy_goal[2];

  NODE_TO_XY(node_number, xy_node)
  NODE_TO_XY(end_node, xy_goal)

  dx = abs(xy_node[0] - xy_goal[0]);
  dy = abs(xy_node[1] - xy_goal[1]);

  return D * (dx + dy) + (D2 - 2 * D) * MIN(dx, dy);
}

uint16_t calculate_g_value(int16_t node_number, int16_t prev_number) {
  uint8_t node_row = node_number / ROW_SIZE;
  uint8_t node_col = node_number % ROW_SIZE;
  uint8_t prev_row = prev_number / ROW_SIZE;
  uint8_t prev_col = prev_number % ROW_SIZE;
  int8_t dx = node_col - prev_col, dy = node_row - prev_row;

  if (abs(dx + dy) == 0 || abs(dx + dy) == 2)
    return pmap[prev_number].g_cost + D2;
  else
    return pmap[prev_number].g_cost + D;
}

void revert_state(void) {
  for (int16_t i = MAP_SIZE - 1; i >= 0; --i) {
    pmap[i].state = NONE;
    pmap[i].prev = NAB;
    pmap[i].f_cost = NCB;
    pmap[i].g_cost = NCB;
  }
}

int16_t path_find(void)
{
  PriorityQueue open;
  int16_t *closed = NULL;
  int16_t cl_size = 0;
  int16_t *neighbors = NULL;
  int8_t nb_size = 0;

  revert_state();

  set_init_node();
  set_end_node();

  pq_init(&open);
  closed = (int16_t *) malloc(sizeof(int16_t));

  if (closed == NULL)
    ESP_LOGE(TAG_PF, "path_find: allocation fail");
  
  if (is_obstacle(init_node) || is_obstacle(end_node))
    return INVALID_NODE;


  pmap[init_node].state = OPEN;  
  enqueue(&open, init_node);

  while (!empty(&open)) {
    
    int16_t current = dequeue(&open);
    pmap[current].state = CLOSED;
    if (current == init_node)
      pmap[current].g_cost = 0;

    //ESP_LOGI(TAG_PF, "Node num check 0: %d", current);
    
    closed[cl_size++] = current;
    closed = (int16_t *) realloc(closed, (cl_size + 1) * sizeof(int16_t));
    if (closed == NULL)
      ESP_LOGE(TAG_PF, "path_find: reallocation fail");
    
    //ESP_LOGI(TAG_PF, "Node num check 1: %d ", current.num);

    if (current == end_node) {
      if (current == init_node)
        pmap[current].prev = current;
      break;
    }
  
    neighbors = get_neighbors(current, &nb_size);

    //ESP_LOGI(TAG_PF, "Node num check 2: %d ", current.num);

    //printf("Enqueued: \n");
    for (--nb_size; nb_size >= 0; --nb_size) {

      if (pmap[neighbors[nb_size]].state == CLOSED || is_obstacle(neighbors[nb_size])) {
        continue;
      }
    
      uint16_t cost = calculate_g_value(neighbors[nb_size], current);
    
      if (cost < pmap[neighbors[nb_size]].g_cost || pmap[neighbors[nb_size]].state != OPEN) {
        pmap[neighbors[nb_size]].g_cost = cost;
        pmap[neighbors[nb_size]].f_cost = cost + (int16_t) round(1.05 * heuristic(neighbors[nb_size]));
        pmap[neighbors[nb_size]].state = OPEN;
        enqueue(&open, neighbors[nb_size]);
        pmap[neighbors[nb_size]].prev = current;
        //printf("%d %d %d %d\n", neighbors[nb_size],
                              //pmap[neighbors[nb_size]].f_cost,
                              //pmap[neighbors[nb_size]].g_cost,
                              //pmap[neighbors[nb_size]].prev);
      }
    }

    free(neighbors);
    neighbors = NULL;
  }
  
  delete(&open);
  free(closed);
  closed = NULL;
  
  if (pmap[end_node].prev != NAB) 
    return pmap[end_node].prev;
  else
    return PATH_NOT_FOUND;

}

int16_t *reconstruct_path(int16_t *len) {
  int16_t *path, temp = 0, k = 0;

  path = (int16_t *) malloc(sizeof(int16_t));
  if (path == NULL) {
    ESP_LOGE(TAG_PF, "reconstruct_path: allocation fail");
    return NULL;
  }

  path[k++] = end_node;
  temp = pmap[end_node].prev;
  while (temp != init_node) {
    path = (int16_t *) realloc(path, (k + 1) * sizeof(int16_t));
    path[k++] = temp;
    temp = pmap[temp].prev;
  }
  path = (int16_t *) realloc(path, (k + 1) * sizeof(int16_t));
  path[k++] = temp;

  *len = k;

  return path;

}