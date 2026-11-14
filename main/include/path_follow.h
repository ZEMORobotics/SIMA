#ifndef PATH_FOLLOW_H
#define PATH_FOLLOW_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct move_s
{
  int16_t move_length;
  int16_t move_angle;
} move_t;

extern volatile move_t current_move;


#endif // PATH_FOLLOW_H