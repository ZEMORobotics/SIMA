#include "path_follow.h"
#include "path_finder.h"
#include "move.h"
#include "stepper_driver_setup.h"

#define DIRECT_LENGTH_MM 500
#define DIAG_LENTH_MM 707

typedef enum direction {
  NORTH =  40,
  SOUTH = -40,
  EAST  =  -1,
  WEST  =   1,
  NEAST =  39,
  NWEST =  41,
  SEAST = -41,
  SWEST = -39
} direction_t;

static int16_t get_direction_angle(direction_t dir) {
  switch (dir)
  {
    case NORTH:
      return 90;
    case SOUTH:
      return -90;
    case EAST:
      return 0;
    case WEST:
      return 180;
    case NEAST:
      return 45;
    case NWEST:
      return 135;
    case SEAST:
      return -45;
    case SWEST:
      return -135;
    default:
      return ESP_FAIL;
  }
}

static esp_err_t move_decode(int16_t current_node, int16_t next_node) {

  direction_t next_dir = next_node - current_node;
  int16_t next_angle = get_direction_angle(next_dir) - current_move.move_angle;

  if (abs(next_dir) == NWEST || abs(next_dir) == NEAST)
    current_move.move_length = DIAG_LENTH_MM;
  else
    current_move.move_angle = DIRECT_LENGTH_MM;
  current_move.move_angle = get_direction_angle(next_dir);

  if (next_angle != 0) {
    if (current_node == init_node)
      rotate_deg(next_angle, true, false);
    else
      rotate_deg(next_angle, false, false);
  }
  
  if (next_node == end_node)
    translate_mm(current_move.move_length, current_move.move_length, false, true);
  else if (current_node == init_node)
    translate_mm(current_move.move_length, current_move.move_length, true, false);
  else
    translate_mm(current_move.move_length, current_move.move_length, false, false);    

  return ESP_OK;
}