#include "inflation_layer.h"
#include "path_finder.h"

static int8_t inf_map[INF_MAP_SIZE] = {0};
volatile int8_t *const pinf = inf_map;

/********************************************************************************/
int8_t is_inflated(int16_t node_number) {
  if (inf_map[node_number])
    return inf_map[node_number];
  else
    return -1;
}
/********************************************************************************/

/********************************************************************************/
void apply_inflation(int8_t inf_depth) {
  int16_t *neighbors = NULL;
  int8_t nb_size = 0;
  int8_t step = 0;

  while (step < inf_depth) {

    for (int16_t i = INF_MAP_SIZE - 1; i >= 0; --i) {
      if (is_obstacle(i))
        continue;

      neighbors = get_neighbors(i, &nb_size);
      for (--nb_size; nb_size >= 0; --nb_size) {
        if ((inf_depth - step) == inf_depth) {
          if (is_obstacle(neighbors[nb_size])) {
            inf_map[i] += (inf_depth - step) * CONFIG_INFLATION_STEP;
            break;
          }
        } else if (inf_map[neighbors[nb_size]] == ((inf_depth - step + 1) * CONFIG_INFLATION_STEP) && !inf_map[i]) {
          inf_map[i] += (inf_depth - step) * CONFIG_INFLATION_STEP;
          break;
        }
      }

      free(neighbors);
      neighbors = NULL;
    }

    ++step;
  }
}
/********************************************************************************/