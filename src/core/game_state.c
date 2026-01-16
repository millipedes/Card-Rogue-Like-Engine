#include "game_state.h"

#include <stdlib.h>

void add_enemy(GameState * state, EnemyState enemy) {
  if (!state->enemies) {
    state->qty_enemies = 1;
    state->enemies = calloc(1, sizeof(EnemyState));
  } else {
    state->qty_enemies++;
    state->enemies = realloc(state->enemies,
        state->qty_enemies * sizeof(EnemyState));
  }
  state->enemies[state->qty_enemies - 1] = enemy;
}

void free_game_state(GameState state) {
  free_self_state(state.self_state);
  if (state.enemies) {
    for (uint8_t i = 0; i < state.qty_enemies; i++) {
      free_enemy_state(state.enemies[i]);
    }
    free(state.enemies);
  }
}
