#ifndef PARSING_ENEMY_STATE_H
#define PARSING_ENEMY_STATE_H

#include <stdint.h>

#include "parsing_common_state.h"
#include "parsing.h"

#define KW_PERCENT "%"

// Top-Level Enemy Specific Keywords
#define KW_ACTION      "Action"
#define KW_CARDINALITY "Cardinality"
#define KW_PROBABILITY "Probability"

// Bitmap is as follows:
// (Action) (Card) (Prob)
// I.e.
//        1      1      1
// => Seen everything
//        0      0      0
// => Seen nothing
#define ENEMY_MOVE_ATTRIBUTE_BIT_NO 0b111
#define ENEMY_MOVE_ACTION_BIT 2
#define ENEMY_MOVE_CARD_BIT   1
#define ENEMY_MOVE_PROB_BIT   0

typedef struct {
  Action * actions;
  double probability;
  double cardinality;
  uint8_t qty_actions;
} Move;

typedef struct {
  Move * moves;
  uint8_t qty_moves;
} MovePool;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

const char * parse_enemy_move_pool(const char * input, MovePool * move_pool);
void free_move_pool(MovePool move_pool);

#ifdef __cplusplus
}
#endif

#endif
