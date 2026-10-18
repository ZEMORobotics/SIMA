#include <stdlib.h>

#define MAP_SIZE 2400
#define ROW_SIZE 40
#define NUMBER_OF_ROWS 60
#define RESOLUTION 50

#define XY_TO_NODE(x, y) ((y * ROW_SIZE) + x)
#define NODE_TO_XY(node_number, node_xy) do { \
                            node_xy[0] = (node_number % ROW_SIZE); \
                            node_xy[1] = (node_number / ROW_SIZE); \
                            } while (0);
#define UPDATE_F_VALUE(node, new_f_value) node = new_f_value

typedef uint8_t FValue;
enum MoveCosts {STRAIGHT = 1, DIAGONAL = 2};

void set_init_node(uint16_t x, uint16_t y);
void set_end_node(uint16_t x, uint16_t y);
uint8_t heuristic(uint16_t node_number);
uint8_t calculate_f_value(uint16_t node_number);
uint8_t get_f_value(uint16_t node_number);
void map_init(void);