#include "map.h"
#include "path_finder.h"

#define MIN(a, b) ((a < b) ? a : b)

static Node init_node, end_node;

void set_init_node(uint16_t x, uint16_t y) {
  init_node.prev = NAB;
  init_node.num = XY_TO_NODE(x, y);
  init_node.h_cost = heuristic(init_node.num);
  init_node.f_cost = init_node.h_cost;
}

void set_end_node(uint16_t x, uint16_t y) {
  end_node.prev = NAB;
  end_node.num = XY_TO_NODE(x, y);
  end_node.h_cost = 0;
  
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

uint8_t get_f_value(uint16_t node_number) {
  return pmap[node_number];
}