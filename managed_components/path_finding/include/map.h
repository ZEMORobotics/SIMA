#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#define MAP_SIZE 2400
#define ROW_SIZE 40
#define NUMBER_OF_ROWS 60
#define RESOLUTION 50

#define NAB -1 // Not accessed before
#define NCB -2 // Not calculated before

#define XY_TO_NODE(x, y) ((y * ROW_SIZE) + x)
#define NODE_TO_XY(node_number, node_xy) do { \
                            node_xy[0] = (node_number % ROW_SIZE); \
                            node_xy[1] = (node_number / ROW_SIZE); \
                            } while (0);

typedef uint8_t FValue;
typedef enum TableElements {FREE, CASTLE, STABLE, QUARRY, MOAT, ROBOT} TableElements;
typedef enum State {OPEN, CLOSED, NONE} State;
typedef struct Node
{
  uint8_t element;
  uint8_t state;
  int16_t prev;
  int16_t num;
  uint16_t g_cost;
  uint16_t f_cost;
} Node;


extern volatile Node *const pmap;

void map_init(void);
uint8_t get_element(uint16_t node_number);
void print_path(int16_t *path, int16_t *len);

#endif // MAP_H