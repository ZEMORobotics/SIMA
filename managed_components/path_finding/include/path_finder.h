#include <stdlib.h>

#define NAB -1 // Not accessed before
#define UPDATE_F_VALUE(node, new_f_value) node = new_f_value

enum MoveCosts {D = 10, D2 = 14};

typedef struct Node
{
  int16_t prev;
  int16_t num;
  uint8_t h_cost;
  uint8_t f_cost;
} Node;

void set_init_node(uint16_t x, uint16_t y);
void set_end_node(uint16_t x, uint16_t y);
int8_t is_obstacle(uint16_t node_number);
uint8_t heuristic(uint16_t node_number);
uint8_t calculate_f_value(uint16_t node_number);
uint8_t calculate_h_value(uint16_t node_number);
uint8_t calculate_g_value(uint16_t node_number);

uint8_t get_f_value(uint16_t node_number);