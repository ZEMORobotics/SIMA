#include "map.h"

Node map[MAP_SIZE];
volatile Node *const pmap = map;

// Yellow oriented
void map_init(void) {

  for (int16_t i = MAP_SIZE - 1; i >= 0; --i) {
    map[i].element = FREE;
    map[i].state = NONE;
    map[i].prev = NAB;
    map[i].num = i;
    map[i].f_cost = NCB;
    map[i].h_cost = NCB;
    map[i].g_cost = NCB;
  }

  // Right side quarries
  for (int8_t i = 1; i <= 4; i++) {
    for (int8_t j = 2 + (i * 10); j < 8 + (i * 10); j++) {
      map[j * ROW_SIZE].element = QUARRY;
      map[j * ROW_SIZE + 1].element = QUARRY;
    }

  }

  // Left side quarries
  for (int8_t i = 1; i <= 4; i++) {
    for (int8_t j = 2 + (i * 10); j < 8 + (i * 10); j++) {
      map[j * ROW_SIZE + ROW_SIZE - 2].element = QUARRY;
      map[j * ROW_SIZE + ROW_SIZE - 1].element = QUARRY;
    }
  }

  // Center quarries
  for (int8_t j = 0; j <= 1; j++) {
    for (int8_t i = 7 + (j * 20); i < 13 + (j * 20); i++) {
      map[(NUMBER_OF_ROWS / 2 - 1) * ROW_SIZE + i].element = QUARRY;
      map[(NUMBER_OF_ROWS / 2) * ROW_SIZE + i].element = QUARRY;
    }
  }

  // Stables(Marking only opponent stables)
  for (int8_t j = 0; j < 2; j++) {
    for (int8_t i = j * 34; i < 6 + (j * 34); i++) {
      map[i].element = STABLE;
      map[ROW_SIZE + i].element = STABLE;
      map[(NUMBER_OF_ROWS - 2) * ROW_SIZE + i].element = STABLE;
      map[(NUMBER_OF_ROWS - 1) * ROW_SIZE + i].element = STABLE;
    }
  }

  // Castles(Including walls)
  for (int8_t j = 0; j < 19; j++) {
    if (j < 12) {
      for (int8_t i = 6; i < 34; i++) {
        map[j * ROW_SIZE + i].element = CASTLE;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = CASTLE;
      }
    } else {
      for (int8_t i = 6 + j - 11; i < 34 - (j - 11); i++) {
        map[j * ROW_SIZE + i].element = CASTLE;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = CASTLE;
      }
    }
  }

  // Moats
  for (int8_t j = 12; j < 21; j++) {
    if (j == 12) {
      map[j * ROW_SIZE + 6].element = MOAT;
      map[j * ROW_SIZE + 33].element = MOAT;
      map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + 6].element = MOAT;
      map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + 33].element = MOAT;
    } else if (j  == 13) {
      for (int8_t i = 5; i < 8; i++) {
        map[j * ROW_SIZE + i].element = MOAT;
        map[j * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
      }
    } else if (j == 14) {
      for (int8_t i = 4; i < 9; i++) {
        map[j * ROW_SIZE + i].element = MOAT;
        map[j * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
      }
    } else if (j < 19) {
      for (int8_t i = 4 + j - 15; i < 9 + j - 14; i++) {
        map[j * ROW_SIZE + i].element = MOAT;
        map[j * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
      }
    } else if (j == 19) {
      for (int8_t i = 8; i < 13; i++) {
        map[j * ROW_SIZE + i].element = MOAT;
        map[j * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
      }
    } else
      for (int8_t i = 9; i < 11; i++) {
        map[j * ROW_SIZE + i].element = MOAT;
        map[j * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = MOAT;
        map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + (ROW_SIZE - 1 - i)].element = MOAT;
      }
  }

  for (int8_t j = 19; j < 23; j++) {
    for (int8_t i = 16; i < 24; i++) {
      map[j * ROW_SIZE + i].element = MOAT;
      map[(NUMBER_OF_ROWS - 1 - j) * ROW_SIZE + i].element = MOAT;
    }
  }

}