#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <stdlib.h>

#define UPDATE_F_VALUE(node, new_f_value) node = new_f_value

typedef enum MoveCosts {D = 10, D2 = 14} MoveCosts;

void set_init_num(uint16_t x, uint16_t y);
void set_end_num(uint16_t x, uint16_t y);
void set_init_node(void);
void set_end_node(void);
int8_t is_obstacle(uint16_t node_number);
uint16_t *get_neighbors(uint16_t node_number, int8_t *len);
uint16_t heuristic(uint16_t node_number);
uint16_t calculate_g_value(uint16_t node_number, uint16_t prev_number);
void revert_state(void);
uint16_t path_find(void);
int16_t *reconstruct_path(int16_t *len);

#endif // PATH_FINDER_H