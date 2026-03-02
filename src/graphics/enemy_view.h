#ifndef ENEMY_VIEW_H
#define ENEMY_VIEW_H

#include <stdbool.h>
#include <stdint.h>

#include <ncurses.h>

#include "core/enemy_state.h"

#include "config_io/stream_enemy.h"

#include "messages.h"

#define MAX_ENEMIES_PER_BATTLE 8

typedef EnemyStates * EnemyStatesRef;

typedef struct {
  uint8_t highlight_index;
  bool is_highlighted;
} EnemyHighlight;

typedef struct {
  WINDOW * art_space[MAX_ENEMIES_PER_BATTLE];
  // Note |art_streams| == |enemy_states|
  ArtStreams art_streams[MAX_ENEMIES_PER_BATTLE];
  int max_enemy_art_width;
  int total_enemy_art_width;
  EnemyHighlight highlight;

  WINDOW * move_space;
  MoveStream move_stream;

  EnemyStatesRef enemy_states_ref;
} EnemyView;

EnemyView init_enemy_view(WINDOW * parent, EnemyStatesRef enemy_states_ref);
EnemyView update_enemy_view(EnemyView enemy_view, BattleMessage message);
void free_enemy_view(EnemyView enemy_view);

#endif
