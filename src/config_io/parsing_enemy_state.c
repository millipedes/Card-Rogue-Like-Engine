#include "parsing_enemy_state.h"

#include <stdlib.h>

void add_move_action(Move * move, Action action) {
  if (!move->actions) {
    move->qty_actions = 1;
    move->actions = calloc(1, sizeof(Action));
  } else {
    move->qty_actions++;
    move->actions = realloc(move->actions,
        move->qty_actions * sizeof(Action));
  }
  move->actions[move->qty_actions - 1] = action;
}

const char * parse_move_actions(const char * input, Move * move) {
  const char * current_input = input;
  const char * final_input = NULL;

  Action tmp = {0};
  while (current_input = parse_action(parse_ws(current_input), &tmp)) {
    final_input = current_input;
    add_move_action(move, tmp);
    tmp = (Action){0};
  }
  return parse_ws(final_input);
}

const char * parse_move(const char * input, Move * move) {
  // see header description
  uint8_t attribute_bitmap = 0;

  const char * result = input;
  const char * tmp_result = NULL;
  // This nonsense is to allow order independence, easier for the user, any
  // questions, again see the header.
  while (attribute_bitmap != ENEMY_MOVE_ATTRIBUTE_BIT_NO && result != NULL) {
    if (tmp_result = parse_keywords(parse_ws(result), 2, KW_ACTION, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_move_actions(parse_ws(tmp_result), move))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << ENEMY_MOVE_ACTION_BIT);
      result = tmp_result;

    } else if (tmp_result = parse_keywords(parse_ws(result), 2, KW_CARDINALITY, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_number(parse_ws(tmp_result), &move->cardinality))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << ENEMY_MOVE_CARD_BIT);
      result = tmp_result;

    } else if (tmp_result = parse_keywords(parse_ws(result), 2, KW_PROBABILITY, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_number(parse_ws(tmp_result), &move->probability))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_PERCENT))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << ENEMY_MOVE_PROB_BIT);
      result = tmp_result;

    } else {
      return NULL;
    }
  }
}

void add_move(MovePool * move_pool, Move move) {
  if (!move_pool->qty_moves) {
    move_pool->qty_moves = 1;
    move_pool->moves = calloc(1, sizeof(Move));
  } else {
    move_pool->qty_moves++;
    move_pool->moves = realloc(move_pool->moves,
        move_pool->qty_moves * sizeof(Move));
  }
  move_pool->moves[move_pool->qty_moves - 1] = move;
}

const char * parse_enemy_move_pool(const char * input, MovePool * move_pool) {
  const char * current_input = input;
  const char * final_input = NULL;

  Move tmp = {0};
  while (current_input = parse_move(parse_ws(current_input), &tmp)) {
    final_input = current_input;
    add_move(move_pool, tmp);
    tmp = (Move){0};
  }
  return parse_ws(final_input);
}

void free_move(Move move) {
  if (move.actions) {
    free(move.actions);
  }
}

void free_move_pool(MovePool move_pool) {
  if (move_pool.moves) {
    for (uint8_t i = 0; i < move_pool.qty_moves; i++) {
      free_move(move_pool.moves[i]);
    }
    free(move_pool.moves);
  }
}
