#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>

#include "enemy_state.h"
#include "self_state.h"

typedef struct {
  SelfState self_state;
  EnemyState * enemies;
  uint8_t qty_enemies;
} GameState;

#endif
