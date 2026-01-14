#include "enemy_state.h"

#include <stdlib.h>

#include "core_utilities.h"

EnemyState init_enemy_state(const char * root_dir) {
  EnemyState enemy_state = {0};
  char * tmp_art_file_name = make_file_name(root_dir, ENEMY_ART_FILE_NAME);
  get_art_lines(&enemy_state.art_lines, &enemy_state.qty_art_lines,
      tmp_art_file_name);
  free(tmp_art_file_name);

  char * tmp_move_pool_file_name = make_file_name(root_dir,
      ENEMY_MOVE_POOL_FILE_NAME);
  char * move_pool_input = dump_file_contents(tmp_move_pool_file_name);
  parse_enemy_move_pool(move_pool_input, &enemy_state.move_pool);
  free(tmp_move_pool_file_name);
  free(move_pool_input);

  return enemy_state;
}
