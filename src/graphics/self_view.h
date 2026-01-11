#ifndef SELF_VIEW_H
#define SELF_VIEW_H

#include <ncurses.h>

#include "core/self_state.h"

typedef SelfState * SelfStateRef;

typedef struct {
  WINDOW * art_space;
  WINDOW * hand_space;
  SelfStateRef self_state_ref;
} SelfView;

SelfView init_self_view(WINDOW * parent, SelfStateRef self_state_ref);
void update_self_view(SelfView self_view);
void free_self_view(SelfView self_view);

#endif
