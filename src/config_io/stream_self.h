#ifndef STREAM_SELF_CARD_H
#define STREAM_SELF_CARD_H

#include <stddef.h>
#include <stdint.h>

#include "core/self_state.h"

#include "parsing_self_state.h"
#include "stream_common.h"

typedef struct {
  char name[MAX_CARD_NAME_LEN];
  char cost[MAX_CARD_NAME_LEN];
  char rarity[MAX_CARD_NAME_LEN];
  char action_texts[MAX_ACTIONS][MAX_ACTION_TEXT];
  uint8_t qty_actions;
} CardStreams;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

CardStreams card_to_stream(const Card card);
ArtStreams self_state_to_art_streams(const SelfState self);

#ifdef __cplusplus
}
#endif

#endif
