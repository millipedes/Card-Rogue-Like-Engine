#ifndef STREAM_SELF_CARD_H
#define STREAM_SELF_CARD_H

#include <stddef.h>
#include <stdint.h>

#include "parsing_self_state.h"

#define MAX_ACTIONS   8
#define MAX_CARD_TEXT 256

// Note that this structure is meant to be completely dis-joint from the self
// state. The reasoning for this being is that its use ought to be more broad
// than that, hence recording the qty_actions here. This stream ought to be
// valid outside of any external state.
typedef struct {
  char name[MAX_CARD_NAME_LEN];
  char cost[MAX_CARD_NAME_LEN];
  char rarity[MAX_CARD_NAME_LEN];
  char action_texts[MAX_ACTIONS][MAX_CARD_TEXT];
  uint8_t qty_actions;
} CardStreams;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

CardStreams card_to_stream(Card card);
size_t max_action_text_len(CardStreams cards_streams);

#ifdef __cplusplus
}
#endif

#endif
