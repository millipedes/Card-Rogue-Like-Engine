#include "game_state.h"

#include <stdlib.h>

void free_game_state(GameState state) {
  free_self_state(state.self_state);
  free_enemy_states(state.enemy_states);
}
