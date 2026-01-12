#include "stream_self_card.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char * copy_string(const char * from) {
  char * into = NULL;
  size_t len = strnlen(from, MAX_CARD_NAME_LEN);
  into = calloc(len + 1, sizeof(char));
  strncpy(into, from, len);
  return into;
}

char * copy_double_to_str(const double from) {
  char buf[MAX_CARD_NAME_LEN] = {0};
  snprintf(buf, sizeof(buf), "%.2f", from);

  return copy_string(buf);
}

char * enemy_debuff_to_stream(CardAction a) {
  "Apply";
  char * qty;
  char * enemy_debuff;
  "to";
  char * target;
  char * duration;
  ".";
}

char * action_to_stream(Card card) {
  char buf[MAX_CARD_TEXT] = {0};
  uint8_t inc = 0;
  for (uint8_t i = 0; i < card.qty_actions; i++) {
    switch (card.actions[i].type) {
      case ENEMY_DEBUFF:
        break;
      case SELF_BUFF:
        break;
      case ATTACK:
        break;
    }
  }
}

CardStreams card_to_stream(Card card) {
  CardStreams cards_streams = {0};

  cards_streams.name = copy_string(card.name);
  cards_streams.cost = copy_double_to_str(card.cost);
  cards_streams.rarity = copy_string(rarity_to_string(card.rarity));

  return cards_streams;
}
