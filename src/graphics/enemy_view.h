#ifndef ENEMY_VIEW_H
#define ENEMY_VIEW_H

#include <ncurses.h>

#include "core/enemy_state.h"

#include "messages.h"

typedef EnemyState * EnemyStateRef;

typedef struct {
  WINDOW * art_space;

  EnemyStateRef enemy_state_ref;
} EnemyView;

EnemyView init_enemy_view(WINDOW * parent, EnemyStateRef enemy_state_ref);
EnemyView update_enemy_view(EnemyView enemy_view, BattleMessage message);

#endif
