#include "map.h"
#include "path_finder.h"
#include "prio_queue.h"
#include "esp_log.h"

#define MIN(a, b) ((a < b) ? a : b)

#define TAG_PF "path_find"

static Node init_node, end_node;

void set_init_node(uint16_t x, uint16_t y) {
  init_node.prev = NAB;
  init_node.num = XY_TO_NODE(x, y);
  init_node.h_cost = heuristic(init_node.num);
  init_node.g_cost = 0;
  init_node.f_cost = init_node.h_cost;
  init_node.state = NONE;
}

void set_end_node(uint16_t x, uint16_t y) { 
  end_node.prev = NAB;
  end_node.num = XY_TO_NODE(x, y);
  end_node.h_cost = 0;
  end_node.g_cost = NCB;
  end_node.f_cost = end_node.g_cost;
  end_node.state = NONE;
}

// Obavezno free da se uradi nakon provere
uint16_t *get_neighbors(uint16_t node_number, int8_t *len) {
  uint16_t *neighbors = (uint16_t *) malloc(8 * sizeof(uint16_t));

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

        //pmap[(node_number + dy[j]) * ROW_SIZE + (node_col + dx[i])].prev = node_number;
        neighbors[k++] = pmap[(node_row + dy[j]) * ROW_SIZE + (node_col + dx[i])].num;
      }
    }
  }

  if (k < 8) {
    neighbors = (uint16_t *) realloc(neighbors, (k + 1) * sizeof(uint16_t));

    if (neighbors == NULL)
      ESP_LOGE(TAG_PF, "get_neighbors: reallocation fail");
  }

  *len = k;

  return neighbors;
}

int8_t is_obstacle(uint16_t node_number) {
  if (pmap[node_number].element == QUARRY || pmap[node_number].element == CASTLE ||
      pmap[node_number].element == ROBOT)
    return 1;
  else
    return 0;
}

uint8_t heuristic(uint16_t node_number) {
  register uint16_t dx, dy;
  register uint16_t xy_node[2], xy_goal[2];

  NODE_TO_XY(node_number, xy_node)
  NODE_TO_XY(end_node.num, xy_goal)

  dx = abs(xy_node[0] - xy_goal[0]);
  dy = abs(xy_node[1] - xy_goal[1]);

  return D * (dx + dy) + (D2 - 2 * D) * MIN(dx, dy);
}

uint8_t calculate_g_value(uint16_t node_number, uint16_t prev_number) {
  uint8_t node_row = node_number / ROW_SIZE;
  uint8_t node_col = node_number % ROW_SIZE;
  uint8_t prev_row = node_number / ROW_SIZE;
  uint8_t prev_col = node_number % ROW_SIZE;
  uint8_t dx = node_col - prev_col, dy = node_row - prev_row;

  if (abs(dx + dy) == 0 || abs(dx + dy) == 2)
    return pmap[prev_number].g_cost + D2;
  else
    return pmap[prev_number].g_cost + D;
}

void revert_state(void) {
  for (uint16_t i = MAX_SIZE; i > 0; --i)
    pmap[i - 1].state = NONE;
}

uint8_t path_find(void)
{
  PriorityQueue open;
  uint16_t *closed;
  int16_t cl_size = 0;
  uint16_t *neighbors;
  int8_t nb_size;

  revert_state();

  pq_init(&open);
  closed = (uint16_t *) malloc(sizeof(uint16_t));

  if (closed == NULL)
    ESP_LOGE(TAG_PF, "path_find: allocation fail");
  
  enqueue(&open, &init_node);
  init_node.state = OPEN;

  while (!empty(&open)) {
    
    Node current = dequeue(&open);
    pmap[current.num].state = CLOSED;

    ESP_LOGI(TAG_PF, "Node num check 0: %d", current.num);
    
    closed[cl_size++] = current.num;
    closed = (uint16_t *) realloc(closed, (cl_size + 1) * sizeof(uint16_t));
    if (closed == NULL)
      ESP_LOGE(TAG_PF, "path_find: reallocation fail");
    
    //ESP_LOGI(TAG_PF, "Node num check 1: %d ", current.num);

    if (current.num == end_node.num) {
      if (current.num == init_node.num)
        pmap[current.num].prev = current.num;
      else
        pmap[current.num].prev = closed[cl_size - 2];

      break;
    }

    neighbors = get_neighbors(current.num, &nb_size);

    //ESP_LOGI(TAG_PF, "Node num check 2: %d ", current.num);

    for (--nb_size; nb_size >= 0; --nb_size) {

      if (pmap[neighbors[nb_size]].state == CLOSED || is_obstacle(neighbors[nb_size])) {
        //ESP_LOGI(TAG_PF, "Check who is closed num: %d", neighbors[nb_size]);
        continue;
      }
    
      uint16_t cost = calculate_g_value(neighbors[nb_size], current.num);

      if (cost < pmap[neighbors[nb_size]].g_cost || pmap[neighbors[nb_size]].state != OPEN) {
        pmap[neighbors[nb_size]].g_cost = cost;
        pmap[neighbors[nb_size]].f_cost = cost + heuristic(neighbors[nb_size]);
        pmap[neighbors[nb_size]].state = OPEN;
        enqueue(&open, (Node const *) &pmap[neighbors[nb_size]]);
        pmap[neighbors[nb_size]].prev = current.num;
      }
    }

    free(neighbors);
    neighbors = NULL;
  }
  
  delete(&open);
  free(closed);
  closed = NULL;
  
  if (pmap[end_node.num].prev != NAB) 
    return 1;
  else
    return 0;

}

uint16_t get_f_value(uint16_t node_number) {
  return pmap[node_number].element;
}