#include "battle_view.h"

#include <string.h>

#include "core/core_utilities.h"

BattleView init_battle_view(GameStateRef game_state_ref) {
  BattleView view = {0};
  int screen_h = 0;
  int screen_w = 0;
  getmaxyx(stdscr, screen_h, screen_w);

  view.enemy_space = newwin(
      screen_h * 75 / 100, // y len
      screen_w * 55 / 100, // x len
      0,                   // y start
      screen_w * 45 / 100  // x start
  );
  box(view.enemy_space, 0, 0);

  view.self_space = newwin(
      screen_h * 75 / 100,
      screen_w * 45 / 100,
      0,
      0
  );
  view.self_view = init_self_view(view.self_space, &game_state_ref->self_state);
  view.enemy_view = init_enemy_view(view.enemy_space, &game_state_ref->enemies[0]);
  box(view.self_space, 0, 0);

  view.info_space = newwin(
      screen_h * 25 / 100,
      screen_w,
      screen_h * 75 / 100,
      0
  );
  box(view.info_space, 0, 0);

  view.game_state_ref = game_state_ref;

  return view;
}

BattleView update_battle_view(BattleView view, BattleMessage message) {
  switch (message) {
    case MSG_STANDBY:
    case MSG_HAND_SELECT_UP:
    case MSG_HAND_SELECT_DOWN:
    default:
      werase(view.enemy_space);
      box(view.enemy_space, 0, 0);
      view.enemy_view = update_enemy_view(view.enemy_view, message);
      wrefresh(view.enemy_space);

      werase(view.self_space);
      box(view.self_space, 0, 0);
      view.self_view = update_self_view(view.self_view, message);
      wrefresh(view.self_space);

      werase(view.info_space);
      box(view.info_space, 0, 0);
      wrefresh(view.info_space);
      break;
  }

  doupdate();
  return view;
}

void free_battle_view(BattleView view) {
  free_enemy_view(view.enemy_view);
  if (view.enemy_space) {
    delwin(view.enemy_space);
  }
  free_self_view(view.self_view);
  if (view.self_space) {
    delwin(view.self_space);
  }
  if (view.info_space) {
    delwin(view.info_space);
  }
}
