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

void add_enemy(GameState * state, EnemyState enemy);
void free_game_state(GameState state);

#endif
