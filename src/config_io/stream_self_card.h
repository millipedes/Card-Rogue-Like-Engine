#ifndef STREAM_SELF_CARD_H
#define STREAM_SELF_CARD_H

#include "parsing_self_state.h"

// typedef struct {
//   char * name;
//   double cost;
//   Rarity rarity;
//   CardAction * actions;
//   uint8_t qty_actions;
// } Card;

#define MAX_CARD_TEXT 256

typedef struct {
  char * name;
  char * cost;
  char * rarity;
  char * action_text;
} CardStreams;

CardStreams card_to_stream(Card card);

#endif
