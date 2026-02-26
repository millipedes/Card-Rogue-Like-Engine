#include "stream_self.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

CardStreams card_to_stream(const Card card) {
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

ArtStreams self_state_to_art_streams(const SelfState self) {
  ArtStreams art_streams = {0};

  char health_buf[ART_MAX_WIDTH] = {0};
  snprintf(health_buf, ART_MAX_WIDTH, "%.2f / %.2f", self.current_health,
      self.max_health);

  art_streams.max_art_width = MAX(
      max_art_width(self.art_lines, self.qty_art_lines),
      MAX(
        strnlen(health_buf, ART_MAX_WIDTH),
        strnlen(self.name, ART_MAX_WIDTH)
      ));
  bool art_is_max_len = art_streams.max_art_width
    == max_art_width(self.art_lines, self.qty_art_lines);

  for (uint8_t i = 0; i < self.qty_art_lines; i++) {
    // If the art is the largest thing it should not be centered
    if (art_is_max_len) {
      art_streams.art[i][0] = CHAR_SPACE;
      strncpy(art_streams.art[i] + 1, self.art_lines[i],
          strnlen(self.art_lines[i], ART_MAX_WIDTH));
    } else {
      center_about_length(art_streams.art[i], self.art_lines[i], art_streams.max_art_width);
    }
  }
  center_about_length(art_streams.name, self.name, art_streams.max_art_width);
  center_about_length(art_streams.health, health_buf, art_streams.max_art_width);
  return art_streams;
}
