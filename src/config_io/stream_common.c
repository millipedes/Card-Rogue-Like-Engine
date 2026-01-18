#include "stream_common.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char * cat_uint_to_str(char * to, const uint8_t from) {
  char buf[MAX_ACTION_TEXT] = {0};
  snprintf(buf, sizeof(buf), "%d", from);
  strncat(to, buf, MAX_ACTION_TEXT);

  return to;
}

void cat_double_to_str(char * to, const double from) {
  char buf[MAX_ACTION_TEXT] = {0};
  snprintf(buf, sizeof(buf), "%.2f", from);
  strncat(to, buf, MAX_ACTION_TEXT);
}

void cat_duration_to_stream(char * to, Duration d) {
  if (d.type != INDEFINITE) {
    strncat(to, KW_FOR, MAX_ACTION_TEXT);
    strncat(to, KW_SPACE, MAX_ACTION_TEXT);

    cat_uint_to_str(to, d.value.remaining_turns);
    strncat(to, KW_SPACE, MAX_ACTION_TEXT);

    strncat(to, d.value.remaining_turns == 1 ? KW_TURN : KW_TURNS, MAX_ACTION_TEXT);
  }
}

void cat_enemy_debuff_to_stream(char * to, Action a) {
  strncat(to, KW_APPLY, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  cat_double_to_str(to, a.value.enemy_debuff.magnitude);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, enemy_debuff_type_to_string(a.value.enemy_debuff.type),
      MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, KW_TO, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, target_to_string(a.target), MAX_ACTION_TEXT);

  if (a.value.enemy_debuff.duration.type != INDEFINITE) {
    strncat(to, KW_SPACE, MAX_ACTION_TEXT);
    cat_duration_to_stream(to, a.value.enemy_debuff.duration);
  }

  strncat(to, KW_PERIOD, MAX_ACTION_TEXT);
}

void cat_self_buff_to_stream(char * to, Action a) {
  strncat(to, KW_GAIN, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  cat_double_to_str(to, a.value.self_buff.magnitude);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, self_buff_type_to_string(a.value.self_buff.type), MAX_ACTION_TEXT);

  if (a.value.self_buff.duration.type != INDEFINITE) {
    strncat(to, KW_SPACE, MAX_ACTION_TEXT);
    cat_duration_to_stream(to, a.value.self_buff.duration);
  }

  strncat(to, KW_PERIOD, MAX_ACTION_TEXT);
}

void cat_attack_to_stream(char * to, Action a) {
  strncat(to, KW_DEAL, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  cat_double_to_str(to, a.value.attack.magnitude);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, KW_BASE, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, KW_DAMAGE, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, KW_TO, MAX_ACTION_TEXT);
  strncat(to, KW_SPACE, MAX_ACTION_TEXT);

  strncat(to, target_to_string(a.target), MAX_ACTION_TEXT);

  strncat(to, KW_PERIOD, MAX_ACTION_TEXT);
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

size_t max_action_text_len(char action_texts[][MAX_ACTION_TEXT],
    uint8_t qty_actions) {
  size_t max_len = 0;
  for (uint8_t i = 0; i < qty_actions; i++) {
    size_t len = strnlen(action_texts[i], MAX_ACTION_TEXT);
    if (len > max_len) {
      max_len = len;
    }
  }
  return max_len;
}
