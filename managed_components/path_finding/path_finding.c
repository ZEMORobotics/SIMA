#include "path_finding.h"

#define MIN(a, b) ((a < b) ? a : b)

static FValue map[MAP_SIZE];
static uint16_t init_node_num, end_node_num;
const volatile FValue *active_node;

void set_init_node(uint16_t x, uint16_t y) {
  init_node_num = XY_TO_NODE(x, y);
}

void set_end_node(uint16_t x, uint16_t y) {
  end_node_num = XY_TO_NODE(x, y);
}

uint8_t heuristic(uint16_t node_number) {
  register uint16_t dx, dy;
  register uint16_t xy_node[2], xy_goal[2];

  NODE_TO_XY(node_number, xy_node)
  NODE_TO_XY(end_node_num, xy_goal)

  dx = abs(xy_node[0] - xy_goal[0]);
  dy = abs(xy_node[1] - xy_goal[1]);

  return STRAIGHT * (dx + dy) + (DIAGONAL - 2 * STRAIGHT) * MIN(dx, dy);
}

uint8_t get_f_value(uint16_t node_number) {
  return map[node_number];
}

void map_init(void) {
  for (int16_t i = MAP_SIZE - 1; i >= 0; --i) {
    map[MAP_SIZE - i - 1] = heuristic(MAP_SIZE - i - 1);
  }
}