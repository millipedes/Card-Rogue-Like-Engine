#include "stream_self_card.h"

#include <stdio.h>
#include <string.h>

CardStreams card_to_stream(Card card) {
  CardStreams cards_streams = {0};

  strncat(cards_streams.name, card.name, MAX_CARD_NAME_LEN - 1);
  cat_double_to_str(cards_streams.cost, card.cost);
  strncat(cards_streams.rarity, rarity_to_string(card.rarity), MAX_CARD_NAME_LEN - 1);
  for (uint8_t i = 0; i < card.qty_actions; i++) {
    if (i > MAX_ACTIONS) {
      fprintf(stderr, "card_to_stream: max number of actions reached, will no "
          "longer be recording actions in output stream");
      break;
    }
    action_to_stream(cards_streams.action_texts[i], card.actions[i]);
    cards_streams.qty_actions++;
  }

  return cards_streams;
}
