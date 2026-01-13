#include "stream_self_card.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char * cat_uint_to_str(char * to, const uint8_t from) {
  char buf[MAX_CARD_NAME_LEN] = {0};
  snprintf(buf, sizeof(buf), "%d", from);
  strncat(to, buf, MAX_CARD_TEXT);

  return to;
}

void cat_double_to_str(char * to, const double from) {
  char buf[MAX_CARD_NAME_LEN] = {0};
  snprintf(buf, sizeof(buf), "%.2f", from);
  strncat(to, buf, MAX_CARD_TEXT);
}

void cat_duration_to_stream(char * to, Duration d) {
  if (d.type != INDEFINITE) {
    strncat(to, KW_FOR, MAX_CARD_TEXT);
    strncat(to, KW_SPACE, MAX_CARD_TEXT);

    cat_uint_to_str(to, d.value.remaining_turns);
    strncat(to, KW_SPACE, MAX_CARD_TEXT);

    strncat(to, d.value.remaining_turns == 1 ? KW_TURN : KW_TURNS, MAX_CARD_TEXT);
  }
}

void cat_enemy_debuff_to_stream(char * to, Action a) {
  strncat(to, KW_APPLY, MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  cat_double_to_str(to, a.value.enemy_debuff.magnitude);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, enemy_debuff_type_to_string(a.value.enemy_debuff.type),
      MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, KW_TO, MAX_CARD_NAME_LEN);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, target_to_string(a.target), MAX_CARD_TEXT);

  if (a.value.enemy_debuff.duration.type != INDEFINITE) {
    strncat(to, KW_SPACE, MAX_CARD_TEXT);
    cat_duration_to_stream(to, a.value.enemy_debuff.duration);
  }

  strncat(to, KW_PERIOD, MAX_CARD_TEXT);
}

void cat_self_buff_to_stream(char * to, Action a) {
  strncat(to, KW_GAIN, MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  cat_double_to_str(to, a.value.self_buff.magnitude);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, self_buff_type_to_string(a.value.self_buff.type), MAX_CARD_TEXT);

  if (a.value.self_buff.duration.type != INDEFINITE) {
    strncat(to, KW_SPACE, MAX_CARD_TEXT);
    cat_duration_to_stream(to, a.value.self_buff.duration);
  }

  strncat(to, KW_PERIOD, MAX_CARD_TEXT);
}

void cat_attack_to_stream(char * to, Action a) {
  strncat(to, KW_DEAL, MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  cat_double_to_str(to, a.value.attack.magnitude);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, KW_BASE, MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, KW_DAMAGE, MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, KW_TO, MAX_CARD_TEXT);
  strncat(to, KW_SPACE, MAX_CARD_TEXT);

  strncat(to, target_to_string(a.target), MAX_CARD_TEXT);

  strncat(to, KW_PERIOD, MAX_CARD_TEXT);
}

void action_to_stream(char * to, Action a) {
  switch (a.type) {
    case ENEMY_DEBUFF:
      cat_enemy_debuff_to_stream(to, a);
      break;
    case SELF_BUFF:
      cat_self_buff_to_stream(to, a);
      break;
    case ATTACK:
      cat_attack_to_stream(to, a);
      break;
  }
}

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

size_t max_action_text_len(CardStreams cards_streams) {
  size_t max_len = 0;
  for (uint8_t i = 0; i < cards_streams.qty_actions; i++) {
    size_t len = strnlen(cards_streams.action_texts[i], MAX_CARD_TEXT);
    if (len > max_len) {
      max_len = len;
    }
  }
  return max_len;
}
