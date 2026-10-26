#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <stdlib.h>

#define PATH_NOT_FOUND -1
#define INVALID_NODE -2

typedef enum MoveCosts {D = 10, D2 = 14} MoveCosts;

/**
 * @brief Set number of initial node
 * @param x x coordinate in mm
 * @param y y coordinate in mm
 */
void set_init_num(uint16_t x, uint16_t y);

/**
 * @brief Set number od ending node
 * @param x x coordinate in mm
 * @param y y coordinate in mm
 */
void set_end_num(uint16_t x, uint16_t y);

/**
 * @brief Initialize initial node structure
 */
void set_init_node(void);

/**
 * @brief Initialize ending node structure
 */
void set_end_node(void);

/**
 * @brief Check whether nodes object is an obstacle
 * @param node_number index of node under check
 * @return
 *  1 - nodes object is an obstacle
 *  0 - it is not
 */
int8_t is_obstacle(int16_t node_number);

/**
 * @brief Give us node numbers of neighboring nodes
 * @param node_number number of the home node
 * @param len pointer to var in which the lenght of the 
 * returning array will be returned
 * @return pointer to array of neighboring node numbers
 */
int16_t *get_neighbors(int16_t node_number, int8_t *len);

/**
 * @brief Calculate the heuristic of the algorithm with octile distance
 * @param node_number number of the node for which the heuristic will
 * be calculated
 * @return calculated heuristic value
 */
uint16_t heuristic(int16_t node_number);

/**
 * @brief Calculate the g-cost of the current node
 * @param node_number number of the node for which the g-cost
 * will be calculated
 * @param prev_number number of the node from which we got to the
 * current node
 * @return calculated g cost
 */
uint16_t calculate_g_value(int16_t node_number, int16_t prev_number);

/**
 * @brief Clean f and g-costs of the nodes before next algorithm call
 */
void revert_state(void);

/**
 * @brief Pathfinding algorithm
 * @return 
 *  - Node number of the previous node of the ending node
 *  - PATH_NOT_FOUND otherwise
 */
int16_t path_find(void);

/**
 * @brief Used for reconstructing found path
 * @param len Pointer to the variable in which the
 * length of the path will be stored
 * @return pointer to the path
 */
int16_t *reconstruct_path(int16_t *len);

#endif // PATH_FINDER_H