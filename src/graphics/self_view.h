#ifndef SELF_VIEW_H
#define SELF_VIEW_H

#include <ncurses.h>

#include "core/self_state.h"
#include "config_io/stream_self_card.h"
#include "messages.h"

typedef SelfState * SelfStateRef;

typedef struct {
  WINDOW * art_space;

  WINDOW * hand_space;
  uint8_t selected;

  WINDOW * card_space;
  CardStreams card_streams;

  SelfStateRef self_state_ref;
} SelfView;

SelfView init_self_view(WINDOW * parent, SelfStateRef self_state_ref);
SelfView update_self_view(SelfView self_view, BattleMessage message);
void free_self_view(SelfView self_view);

#endif
