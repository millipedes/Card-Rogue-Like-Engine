#ifndef BATTLE_VIEW_H
#define BATTLE_VIEW_H

#include <ncurses.h>

#include "core/game_state.h"

#include "messages.h"
#include "enemy_view.h"
#include "self_view.h"

typedef GameState * GameStateRef;

typedef struct {
  WINDOW * enemy_space;
  EnemyView enemy_view;

  WINDOW * self_space;
  SelfView self_view;

  WINDOW * info_space;

  GameStateRef game_state_ref;
} BattleView;

BattleView init_battle_view(GameStateRef game_state_ref);
BattleView update_battle_view(BattleView view, BattleMessage message);
void free_battle_view(BattleView view);

#endif
