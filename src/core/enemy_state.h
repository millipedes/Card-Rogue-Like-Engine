#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

#include "config_io/parsing_enemy_state.h"
#include "config_io/text_stream.h"

#define ENEMY_ART_FILE_NAME       "art.txt"
#define ENEMY_MOVE_POOL_FILE_NAME "move_pool.txt"

typedef struct {
  TextStream art_lines; // pre-grabbing lines leads to less friction with ncurses

  MovePool move_pool;
} EnemyState;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

EnemyState init_enemy_state(const char * root_dir);
void free_enemy_state(EnemyState enemy_state);

#ifdef __cplusplus
}
#endif

#endif
