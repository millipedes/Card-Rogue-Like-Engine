#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>

#include "enemy_state.h"
#include "self_state.h"

typedef struct {
  SelfState self_state;
  EnemyStates enemy_states;
} GameState;

void free_game_state(GameState state);

#endif
