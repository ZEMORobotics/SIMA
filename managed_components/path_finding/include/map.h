#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#define MAP_SIZE 2400
#define ROW_SIZE 40
#define NUMBER_OF_ROWS 60
#define RESOLUTION 50

#define NAB 32767 // Not accessed before
#define NCB 65535 // Not calculated before

#define XY_TO_NODE(x, y) ((y * ROW_SIZE) + x)
#define NODE_TO_XY(node_number, node_xy) do { \
                            node_xy[0] = (node_number % ROW_SIZE); \
                            node_xy[1] = (node_number / ROW_SIZE); \
                            } while (0);

typedef enum TableElements {FREE, CASTLE, STABLE, QUARRY, MOAT, ROBOT} TableElements;

/// State to showcase whether node is visited by the algorithm or not, or it was not even used
typedef enum State {OPEN, CLOSED, NONE} State;
typedef struct Node
{
  uint8_t element;
  uint8_t state;
  int16_t prev;
  uint16_t g_cost;
  uint16_t f_cost;
} Node;

extern volatile Node *const pmap;

/**
 * @brief Initialize nodes of the map
 */
void map_init(void);

/**
 * @brief Retrieve element of the node
 * @param node_number number of the node whose element
 * we want to get
 * @return value of the nodes element
 */
uint8_t get_element(int16_t node_number);

/**
 * @brief Set element of the node
 * @param node_number number of the node whose element
 * we want to set
 * @param element - element we want to set
 */
void set_element(int16_t node_number, TableElements element);

/**
 * @brief Debug function used for map printing
 * @param path pointer to the path received by
 * reconstruct_path
 * @param len pointer of the variable in which the
 * length of the path is stored
 */
void print_path(int16_t *path, int16_t *len);

#endif // MAP_H