#include <stdlib.h>

#define UPDATE_F_VALUE(node, new_f_value) node = new_f_value

typedef enum MoveCosts {D = 10, D2 = 14} MoveCosts;

void set_init_node(uint16_t x, uint16_t y);
void set_end_node(uint16_t x, uint16_t y);
int8_t is_obstacle(uint16_t node_number);
uint16_t *get_neighbors(uint16_t node_number);
uint8_t heuristic(uint16_t node_number);
uint8_t calculate_g_value(uint16_t node_number, uint16_t prev_number);
// uint8_t calculate_f_value(uint8_t g_value, uint8_t h_value);

uint16_t get_f_value(uint16_t node_number);