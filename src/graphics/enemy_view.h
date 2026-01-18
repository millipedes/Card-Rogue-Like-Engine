#ifndef ENEMY_VIEW_H
#define ENEMY_VIEW_H

#include <ncurses.h>

#include "core/enemy_state.h"
#include "config_io/stream_enemy_move.h"

#include "messages.h"

typedef EnemyState * EnemyStateRef;

typedef struct {
  WINDOW * art_space;

  WINDOW * move_space;
  MoveStream move_stream;

  EnemyStateRef enemy_state_ref;
} EnemyView;

EnemyView init_enemy_view(WINDOW * parent, EnemyStateRef enemy_state_ref);
EnemyView update_enemy_view(EnemyView enemy_view, BattleMessage message);
void free_enemy_view(EnemyView enemy_view);

#endif
