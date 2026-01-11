#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include <ncurses.h>

#include "self_view.h"

typedef struct {
  WINDOW * enemy_space;
  WINDOW * self_space;
  SelfView self_view;
  WINDOW * info_space;
} BattleView;

BattleView init_battle_view(SelfStateRef self_state_ref);
void update_battle_view(BattleView view);
void free_battle_view(BattleView view);

#endif
