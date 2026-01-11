#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "core/self_state.h"

#include "graphics/battle_view.h"
#include "graphics/core_graphics.h"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./card_rl source_file_root_dir/");
    exit(1);
  }

  srand(time(NULL));

  SelfState self_state = init_self_state(argv[1]);
  self_state = start_battle(self_state, STARTING_HAND_SIZE);

  init_ncurses();
  BattleView view = init_battle_view(&self_state);

  int prev_h = 0;
  int prev_w = 0;
  getmaxyx(stdscr, prev_h, prev_w);

  char ch = '\0';
  timeout(16);
  while (true) {
    int curr_h;
    int curr_w;
    getmaxyx(stdscr, curr_h, curr_w);
    update_battle_view(view);

    ch = getch();
    if (ch == 'q') {
      break;
    }
    if (ch == KEY_RESIZE || curr_h != prev_h || curr_w != prev_w) {
      prev_h = curr_h;
      prev_w = curr_w;
      clear();
      refresh();
      free_battle_view(view);
      view = init_battle_view(&self_state);
    }
  }

  free_battle_view(view);
  free_self_state(self_state);
  end_ncurses();
  return 0;
}
