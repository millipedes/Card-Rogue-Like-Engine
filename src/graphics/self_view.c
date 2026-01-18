#include "self_view.h"

#include <string.h>

#include "core/core_utilities.h"

SelfView init_self_view(WINDOW * parent, SelfStateRef self_state_ref) {
  SelfView self_view = {0};
  self_view.self_state_ref = self_state_ref;

  int parent_h = 0;
  int parent_w = 0;
  getmaxyx(parent, parent_h, parent_w);

  self_view.art_space = derwin(parent,
      self_state_ref->qty_art_lines + 3,
      max_art_width(self_state_ref->art_lines, self_state_ref->qty_art_lines) + 4,
      parent_h * 25 / 100,
      parent_w * 35 / 100
  );
  box(self_view.art_space, 0, 0);

  self_view.hand_space = derwin(parent,
      self_state_ref->qty_hand + 2,
      max_hand_name_width(*self_state_ref) + 4,
      parent_h * 50 / 100,
      1
  );

  self_view.card_space = derwin(parent,
      self_view.card_streams.qty_actions + 5, // Name, cost, rarity, border
      max_action_text_len(self_view.card_streams.action_texts, self_view.card_streams.qty_actions) + 4,
      parent_h * 50 / 100,
      max_hand_name_width(*self_state_ref) + 5
  );

  return self_view;
}

void draw_self_art(SelfView self_view) {
  werase(self_view.art_space);
  for (uint8_t i = 0; i < self_view.self_state_ref->qty_art_lines; i++) {
    mvwprintw(self_view.art_space,
        i + 1,
        1,
        " %s ", self_view.self_state_ref->art_lines[i]);
  }
  box(self_view.art_space, 0, 0);
  wrefresh(self_view.art_space);
}

void draw_hand(SelfView self_view) {
  werase(self_view.hand_space);
  for (uint8_t i = 0; i < self_view.self_state_ref->qty_hand; i++) {
    if (i == self_view.selected) {
      wattron(self_view.hand_space, A_REVERSE);
    }
    mvwprintw(self_view.hand_space,
        i + 1,
        1,
        " %s ", self_view.self_state_ref->hand[i]->name);
    wattroff(self_view.hand_space, A_REVERSE);
  }
  box(self_view.hand_space, 0, 0);
  wrefresh(self_view.art_space);
}

void draw_card_streams(SelfView self_view) {
  werase(self_view.card_space);
  if (strncmp(self_view.card_streams.name,
              self_view.self_state_ref->hand[self_view.selected]->name,
              MAX_CARD_NAME_LEN)) {
    self_view.card_streams = (CardStreams){0};
    self_view.card_streams
      = card_to_stream(*self_view.self_state_ref->hand[self_view.selected]);
    wresize(self_view.card_space, self_view.card_streams.qty_actions + 5,
        max_action_text_len(self_view.card_streams.action_texts, self_view.card_streams.qty_actions) + 4);
  }
  mvwprintw(self_view.card_space,
      1,
      1,
      " %s ", self_view.card_streams.name
  );
  mvwprintw(self_view.card_space,
      2,
      1,
      " %s ", self_view.card_streams.cost
  );
  mvwprintw(self_view.card_space,
      3,
      1,
      " %s ", self_view.card_streams.rarity
  );
  for (uint8_t i = 0; i < self_view.card_streams.qty_actions; i++) {
    mvwprintw(self_view.card_space,
        4 + i,
        1,
        " %s ", self_view.card_streams.action_texts[i]
    );
  }

  box(self_view.card_space, 0, 0);
  wrefresh(self_view.card_space);
}

SelfView draw_hand_new_select(SelfView self_view, int dir) {
  werase(self_view.hand_space);
  size_t qty_hand = self_view.self_state_ref->qty_hand;
  self_view.selected = (self_view.selected + dir + qty_hand) % qty_hand;
  for (uint8_t i = 0; i < self_view.self_state_ref->qty_hand; i++) {
    if (i == self_view.selected) {
      wattron(self_view.hand_space, A_REVERSE);
    }
    mvwprintw(self_view.hand_space,
        i + 1,
        1,
        " %s ", self_view.self_state_ref->hand[i]->name
    );
    wattroff(self_view.hand_space, A_REVERSE);
  }
  box(self_view.hand_space, 0, 0);
  wrefresh(self_view.hand_space);
  return self_view;
}

SelfView update_self_view(SelfView self_view, BattleMessage message) {
  switch (message) {
    case MSG_HAND_SELECT_DOWN:
      draw_self_art(self_view);
      self_view = draw_hand_new_select(self_view, 1);
      draw_card_streams(self_view);
      break;
    case MSG_HAND_SELECT_UP:
      draw_self_art(self_view);
      self_view = draw_hand_new_select(self_view, -1);
      draw_card_streams(self_view);
      break;
    case MSG_STANDBY:
    default:
      draw_self_art(self_view);
      draw_hand(self_view);
      draw_card_streams(self_view);
      break;
  }
  return self_view;
}

void free_self_view(SelfView self_view) {
  if (self_view.art_space) {
    delwin(self_view.art_space);
  }
  if (self_view.hand_space) {
    delwin(self_view.hand_space);
  }
}
