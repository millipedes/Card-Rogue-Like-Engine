#include "enemy_state.h"

#include <stdlib.h>

EnemyState init_enemy_state(const char * root_dir) {
  EnemyState enemy_state = {0};
  char * tmp_art_file_name = make_file_name(root_dir, ENEMY_ART_FILE_NAME);
  dump_file_contents(enemy_state.art_lines, tmp_art_file_name);
  free(tmp_art_file_name);

  char * tmp_move_pool_file_name = make_file_name(root_dir,
      ENEMY_MOVE_POOL_FILE_NAME);
  TextStream move_pool_input = {0};
  dump_file_contents(move_pool_input, tmp_move_pool_file_name);
  parse_enemy_move_pool(move_pool_input, &enemy_state.move_pool);
  free(tmp_move_pool_file_name);

  return enemy_state;
}

void free_enemy_state(EnemyState enemy_state) {
  free_move_pool(enemy_state.move_pool);
}
