#include "enemy_view.h"

#include <math.h>
#include <string.h>
#include <sys/param.h>

#include "core/core_utilities.h"

void add_move_stream(EnemyView * enemy_view, MoveStream move_stream) {
}

EnemyView init_enemy_view(WINDOW * parent, EnemyStatesRef enemy_states_ref) {
  EnemyView enemy_view = {0};
  enemy_view.enemy_states_ref = enemy_states_ref;

  int parent_h = 0;
  int parent_w = 0;
  getmaxyx(parent, parent_h, parent_w);

  for (uint8_t i = 0; i < enemy_states_ref->qty_enemies; i++) {
    enemy_view.art_streams[i]
      = enemy_state_to_art_streams(enemy_view.enemy_states_ref->enemies[i]);
    if (enemy_view.max_enemy_art_width < enemy_view.art_streams[i].max_art_width) {
      enemy_view.max_enemy_art_width = enemy_view.art_streams[i].max_art_width;
    }

    enemy_view.total_enemy_art_width += enemy_view.art_streams[i].max_art_width + 4;
  }

  for (uint8_t i = 0; i < enemy_view.enemy_states_ref->qty_enemies; i++) {
    int x = round((double)parent_w / ((double)enemy_view.enemy_states_ref->qty_enemies + 1.0)
            * ((double)enemy_view.art_streams[i].max_art_width * ((double)i + 1.0)
              / (double)enemy_view.max_enemy_art_width));
    enemy_view.art_space[i] = derwin(parent,
        enemy_view.enemy_states_ref->enemies[i].qty_art_lines + 4,
        enemy_view.art_streams[i].max_art_width + 4,
        parent_h * 25 / 100,
        x
    );
    box(enemy_view.art_space[i], 0, 0);

    // enemy_view.move_stream = move_to_stream(enemy_states_ref->enemies[i].move_pool.moves[0]);
    // enemy_view.move_space = derwin(parent,
    //     enemy_view.move_stream.qty_actions + 2,
    //     max_action_text_len(enemy_view.move_stream.action_texts, enemy_view.move_stream.qty_actions) + 4,
    //     parent_h * 50 / 100,
    //     x
    // );
    // box(enemy_view.move_space, 0, 0);
  }



  return enemy_view;
}

void draw_enemy_art(EnemyView enemy_view) {
  for (uint8_t i = 0; i < enemy_view.enemy_states_ref->qty_enemies; i++) {
    werase(enemy_view.art_space[i]);
    for (uint8_t j = 0; j < enemy_view.enemy_states_ref->enemies[i].qty_art_lines; j++) {
      mvwprintw(enemy_view.art_space[i],
          j + 1,
          1,
          "%s", enemy_view.art_streams[i].art[j]);
    }
    mvwprintw(enemy_view.art_space[i],
        enemy_view.enemy_states_ref->enemies[i].qty_art_lines + 1,
        1,
        "%s", enemy_view.art_streams[i].name);
    mvwprintw(enemy_view.art_space[i],
        enemy_view.enemy_states_ref->enemies[i].qty_art_lines + 2,
        1,
        "%s", enemy_view.art_streams[i].health);
    if (enemy_view.highlight.is_highlighted) {
      wattron(enemy_view.art_space[i], A_REVERSE);
    }
    box(enemy_view.art_space[i], 0, 0);
    wattroff(enemy_view.art_space[i], A_REVERSE);
    wrefresh(enemy_view.art_space[i]);
  }
}

void draw_move_stream(EnemyView enemy_view) {
  // werase(enemy_view.move_space);
  // for (uint8_t i = 0; i < enemy_view.move_stream.qty_actions; i++) {
  //   mvwprintw(enemy_view.move_space,
  //       1 + i,
  //       1,
  //       " %s ", enemy_view.move_stream.action_texts[i]
  //   );
  // }

  // box(enemy_view.move_space, 0, 0);
  // wrefresh(enemy_view.move_space);
}

EnemyView update_enemy_view(EnemyView enemy_view, BattleMessage message) {
  switch (message) {
    case BATTLE_MSG_HAND_SELECT_UP:
    case BATTLE_MSG_HAND_SELECT_DOWN:
    case BATTLE_MSG_HAND_UNSELECT_CARD:
      enemy_view.highlight.is_highlighted = false;
      draw_enemy_art(enemy_view);
      draw_move_stream(enemy_view);
      break;
    case BATTLE_MSG_HAND_SELECT_CARD:
      enemy_view.highlight.is_highlighted = true;
      draw_enemy_art(enemy_view);
      draw_move_stream(enemy_view);
      break;
    default:
      draw_enemy_art(enemy_view);
      draw_move_stream(enemy_view);
      break;
  }
  return enemy_view;
}

void free_enemy_view(EnemyView enemy_view) {
  for (uint8_t i = 0; i < enemy_view.enemy_states_ref->qty_enemies; i++) {
    if (enemy_view.art_space[i]) {
      delwin(enemy_view.art_space[i]);
    }
  }
  if (enemy_view.move_space) {
    delwin(enemy_view.move_space);
  }
}
