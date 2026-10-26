#ifndef INFLATION_LAYER_H
#define INFLATION_LAYER_H

#include <stdio.h>

#define INF_MAP_SIZE 2400
#define NOT_INFLATED -1
#define INF_STEP 2

extern volatile int8_t *const pinf;

/**
 * @brief See if a node is inflated
 * @param node_number number of the node to
 * be checked
 * @return
 *  - Inflation value if it is
 *  - NOT_INFLATED if is not
 */
int8_t is_inflated(int16_t node_number);

/**
 * @brief Apply inflation to right nodes
 * @param inf_depth number of nodes near
 * obstacles to be inflated
 */
void apply_inflation(int8_t inf_depth);

#endif // INFLATION_LAYER_H