#include "map.h"
#include "path_finder.h"

#define MIN(a, b) ((a < b) ? a : b)

static Node init_node, end_node;

void set_init_node(uint16_t x, uint16_t y) {
  init_node.prev = NAB;
  init_node.num = XY_TO_NODE(x, y);
  init_node.h_cost = heuristic(init_node.num);
  init_node.g_cost = 0;
  init_node.f_cost = init_node.h_cost;
}

void set_end_node(uint16_t x, uint16_t y) { 
  end_node.prev = NAB;
  end_node.num = XY_TO_NODE(x, y);
  end_node.h_cost = 0;
  end_node.g_cost = NCB;
  end_node.f_cost = end_node.g_cost;
}

// Obavezno free da se uradi nakon pro
uint16_t *get_neighbors(uint16_t node_number) {
  uint16_t *neighbors = (uint16_t *) malloc(8 * sizeof(uint16_t));
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

        neighbors[k++] = pmap[(node_number + dy[j]) * ROW_SIZE + (node_col + dx[i])].num;
      }
    }
  }

  if (k < 8) {
    neighbors = (uint16_t *) realloc(neighbors, (k + 1) * sizeof(uint16_t));
  }

  return neighbors;
}

int8_t is_obstacle(uint16_t node_number) {
  if (pmap[node_number].element == QUARRY || pmap[node_number].element == CASTLE ||
      pmap[node_number].element == ROBOT)
    return -1;
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

uint8_t get_f_value(uint16_t node_number) {
  return pmap[node_number].element;
}