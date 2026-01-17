#include "enemy_view.h"

#include "core/core_utilities.h"

EnemyView init_enemy_view(WINDOW * parent, EnemyStateRef enemy_state_ref) {
  EnemyView enemy_view = {0};
  enemy_view.enemy_state_ref = enemy_state_ref;

  int parent_h = 0;
  int parent_w = 0;
  getmaxyx(parent, parent_h, parent_w);

  enemy_view.art_space = derwin(parent,
      enemy_state_ref->qty_art_lines + 3,
      max_art_width(enemy_state_ref->art_lines, enemy_state_ref->qty_art_lines) + 4,
      parent_h * 50 / 100,
      parent_w * 50 / 100
  );
  box(enemy_view.art_space, 0, 0);

  return enemy_view;
}

void draw_enemy_art(EnemyView enemy_view) {
  werase(enemy_view.art_space);
  for (uint8_t i = 0; i < enemy_view.enemy_state_ref->qty_art_lines; i++) {
    mvwprintw(enemy_view.art_space,
        i + 1,
        1,
        " %s ", enemy_view.enemy_state_ref->art_lines[i]);
  }
  box(enemy_view.art_space, 0, 0);
  wrefresh(enemy_view.art_space);
}

EnemyView update_enemy_view(EnemyView enemy_view, BattleMessage message) {
  switch (message) {
    case MSG_STANDBY:
    default:
      draw_enemy_art(enemy_view);
      break;
  }
  return enemy_view;
}
