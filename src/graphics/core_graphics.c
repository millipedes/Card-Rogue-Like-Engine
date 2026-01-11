#include "core_graphics.h"

#include <ncurses.h>

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  clear();
  refresh();
}

void end_ncurses() {
  endwin();
}
