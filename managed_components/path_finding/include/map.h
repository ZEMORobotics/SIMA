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
typedef enum State {OPEN, CLOSED} State;
typedef struct Node
{
  TableElements element;
  State state;
  int16_t prev;
  int16_t num;
  uint8_t h_cost;
  uint8_t g_cost;
  uint8_t f_cost;
} Node;


extern volatile Node *const pmap;

void map_init(void);