#ifndef ENEMY_VIEW_H
#define ENEMY_VIEW_H

#include <stdbool.h>
#include <stdint.h>

#include <ncurses.h>

#include "core/enemy_state.h"
#include "config_io/stream_enemy.h"

#include "messages.h"

typedef EnemyState * EnemyStateRef;

typedef struct {
  uint8_t highlight_index;
  bool is_highlighted;
} EnemyHighlight;

typedef struct {
  WINDOW * art_space;
  ArtStreams art_streams;
  EnemyHighlight highlight;

  WINDOW * move_space;
  MoveStream move_stream;

  EnemyStateRef enemy_state_ref;
} EnemyView;

EnemyView init_enemy_view(WINDOW * parent, EnemyStateRef enemy_state_ref);
EnemyView update_enemy_view(EnemyView enemy_view, BattleMessage message);
void free_enemy_view(EnemyView enemy_view);

#endif
