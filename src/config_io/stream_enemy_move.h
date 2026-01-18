#ifndef ENEMY_MOVE_H
#define ENEMY_MOVE_H

#include <stddef.h>
#include <stdint.h>

#include "config_io/parsing_enemy_state.h"

#include "stream_common.h"

typedef struct {
  char action_texts[MAX_ACTIONS][MAX_ACTION_TEXT];
  uint8_t qty_actions;
} MoveStream;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

MoveStream move_to_stream(Move move);

#ifdef __cplusplus
}
#endif

#endif
