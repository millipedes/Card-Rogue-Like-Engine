#include "enemy_state.h"

#include <stdlib.h>

#include "core_utilities.h"

EnemyState init_enemy_state(const char * root_dir) {
  EnemyState enemy_state = {0};
  enemy_state.name = make_entity_name(root_dir, ENEMY_DIR_PREFIX);

  char * tmp_art_file_name = make_file_name(root_dir, ENEMY_ART_FILE_NAME);
  get_art_lines(&enemy_state.art_lines, &enemy_state.qty_art_lines,
      tmp_art_file_name);
  free(tmp_art_file_name);

  EnemyStats enemy_stats = {0};
  char * tmp_stats_file_name = make_file_name(root_dir, ENEMY_STATS_FILE_NAME);
  char * stats_input = dump_file_contents(tmp_stats_file_name);
  parse_enemy_stats(stats_input, &enemy_stats);
  free(tmp_stats_file_name);
  free(stats_input);

  enemy_state.health = enemy_stats.health;

  char * tmp_move_pool_file_name = make_file_name(root_dir,
      ENEMY_MOVE_POOL_FILE_NAME);
  char * move_pool_input = dump_file_contents(tmp_move_pool_file_name);
  parse_enemy_move_pool(move_pool_input, &enemy_state.move_pool);
  free(tmp_move_pool_file_name);
  free(move_pool_input);

  return enemy_state;
}

void free_enemy_state(EnemyState enemy_state) {
  if (enemy_state.name) {
    free(enemy_state.name);
  }

  if (enemy_state.art_lines) {
    for (uint8_t i = 0; i < enemy_state.qty_art_lines; i++) {
      free(enemy_state.art_lines[i]);
    }
    free(enemy_state.art_lines);
  }
  free_move_pool(enemy_state.move_pool);
}

void add_enemy(EnemyStates * states, EnemyState enemy) {
  if (!states->enemies) {
    states->qty_enemies = 1;
    states->enemies = calloc(1, sizeof(EnemyState));
  } else {
    states->qty_enemies++;
    states->enemies = realloc(states->enemies,
        states->qty_enemies * sizeof(EnemyState));
  }
  states->enemies[states->qty_enemies - 1] = enemy;
}

void free_enemy_states(EnemyStates states) {
  if (states.enemies) {
    for (uint8_t i = 0; i < states.qty_enemies; i++) {
      free_enemy_state(states.enemies[i]);
    }
    free(states.enemies);
  }
}
