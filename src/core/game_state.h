#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>

#include "enemy.h"
#include "self_state.h"

typedef struct {
  SelfState self_state;
  Enemy * enemies;
  uint8_t qty_enemies;
} GameState;

#endif
