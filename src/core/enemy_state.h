#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

#include "config_io/parsing_enemy_state.h"

#define ENEMY_ART_FILE_NAME       "art.txt"
#define ENEMY_MOVE_POOL_FILE_NAME "move_pool.txt"

typedef struct {
  char ** art_lines; // pre-grabbing lines leads to less friction with ncurses
  uint8_t qty_art_lines;

  MovePool move_pool;
} EnemyState;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

EnemyState init_enemy_state(const char * root_dir);

#ifdef __cplusplus
}
#endif

#endif
