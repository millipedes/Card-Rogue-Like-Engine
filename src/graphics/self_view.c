#include "self_view.h"

SelfView init_self_view(WINDOW * parent, SelfStateRef self_state_ref) {
  SelfView self_view = {0};
  self_view.self_state_ref = self_state_ref;

  int parent_h = 0;
  int parent_w = 0;
  getmaxyx(parent, parent_h, parent_w);

  self_view.art_space = subwin(parent,
      self_state_ref->qty_art_lines + 3,
      max_art_width(*self_state_ref) + 4,
      parent_h * 25 / 100,
      parent_w * 25 / 100
  );
  box(self_view.art_space, 0, 0);

  self_view.hand_space = subwin(parent,
      self_state_ref->qty_hand + 2,
      max_hand_name_width(*self_state_ref) + 4,
      parent_h * 50 / 100,
      parent_w * 25 / 100
  );

  return self_view;
}

void draw_art(SelfView self_view) {
  for (uint8_t i = 0; i < self_view.self_state_ref->qty_art_lines; i++) {
    mvwprintw(self_view.art_space,
        i + 1,
        1,
        " %s ", self_view.self_state_ref->art_lines[i]);
  }
}

void draw_hand(SelfView self_view) {
  for (uint8_t i = 0; i < self_view.self_state_ref->qty_hand; i++) {
    mvwprintw(self_view.hand_space,
        i + 1,
        1,
        " %s ", self_view.self_state_ref->hand[i]->name);
  }
}

void update_self_view(SelfView self_view) {
  werase(self_view.art_space);
  box(self_view.art_space, 0, 0);
  draw_art(self_view);
  wrefresh(self_view.art_space);

  werase(self_view.hand_space);
  box(self_view.hand_space, 0, 0);
  draw_hand(self_view);
  wrefresh(self_view.hand_space);
}

void free_self_view(SelfView self_view) {
  if (self_view.art_space) {
    delwin(self_view.art_space);
  }
  if (self_view.hand_space) {
    delwin(self_view.hand_space);
  }
}
