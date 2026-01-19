#include "core_graphics.h"

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  clear();
  refresh();
}

WINDOW * autosize_window(WINDOW * parent, TextStream tstream, uint8_t per_y,
    uint8_t per_x) {
  int parent_h = 0;
  int parent_w = 0;
  getmaxyx(parent, parent_h, parent_w);
  return derwin(parent,
      max_tstream_height(tstream) + 2,
      max_tstream_len(tstream) + 4,
      parent_h * per_y / 100,
      parent_w * per_x / 100
  );
}

void end_ncurses() {
  endwin();
}
