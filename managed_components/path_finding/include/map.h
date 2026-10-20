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

typedef uint8_t FValue;
typedef enum TableElements {FREE, CASTLE, STABLE, QUARRY, MOAT, ROBOT} TableElements;

extern volatile TableElements *const pmap;

void map_init(void);