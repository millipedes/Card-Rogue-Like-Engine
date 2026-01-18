#include "stream_enemy_move.h"

#include <stdio.h>

MoveStream move_to_stream(Move move) {
  MoveStream move_stream = {0};

  for (uint8_t i = 0; i < move.qty_actions; i++) {
    if (i > MAX_ACTIONS) {
      fprintf(stderr, "move_to_stream: max number of actions reached, will no "
          "longer be recording actions in output stream");
      break;
    }
    action_to_stream(move_stream.action_texts[i], move.actions[i]);
    move_stream.qty_actions++;
  }

  return move_stream;
}
