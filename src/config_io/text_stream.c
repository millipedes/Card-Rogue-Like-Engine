#include "text_stream.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void cat_uint_to_tstream(char * to, const uint8_t from) {
  char buf[MAX_TEXT_STREAM_LEN] = {0};
  snprintf(buf, sizeof(buf), "%d", from);
  strncat(to, buf, MAX_TEXT_STREAM_LEN);
}

void cat_double_to_tstream(char * to, const double from) {
  char buf[MAX_TEXT_STREAM_LEN] = {0};
  snprintf(buf, sizeof(buf), "%.2f", from);
  strncat(to, buf, MAX_TEXT_STREAM_LEN);
}

void cat_duration_to_tstream(char * to, Duration d) {
  if (d.type != INDEFINITE) {
    strncat(to, KW_FOR, MAX_TEXT_STREAM_LEN);
    strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

    cat_uint_to_tstream(to, d.value.remaining_turns);
    strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

    strncat(to, d.value.remaining_turns == 1 ? KW_TURN : KW_TURNS, MAX_TEXT_STREAM_LEN);
  }
}

void cat_enemy_debuff_to_tstream(char * to, Action a) {
  strncat(to, KW_APPLY, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  cat_double_to_tstream(to, a.value.enemy_debuff.magnitude);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, enemy_debuff_type_to_string(a.value.enemy_debuff.type),
      MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, KW_TO, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, target_to_string(a.target), MAX_TEXT_STREAM_LEN);

  if (a.value.enemy_debuff.duration.type != INDEFINITE) {
    strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);
    cat_duration_to_tstream(to, a.value.enemy_debuff.duration);
  }

  strncat(to, KW_PERIOD, MAX_TEXT_STREAM_LEN);
}

void cat_self_buff_to_tstream(char * to, Action a) {
  strncat(to, KW_GAIN, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  cat_double_to_tstream(to, a.value.self_buff.magnitude);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, self_buff_type_to_string(a.value.self_buff.type), MAX_TEXT_STREAM_LEN);

  if (a.value.self_buff.duration.type != INDEFINITE) {
    strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);
    cat_duration_to_tstream(to, a.value.self_buff.duration);
  }

  strncat(to, KW_PERIOD, MAX_TEXT_STREAM_LEN);
}

void cat_attack_to_tstream(char * to, Action a) {
  strncat(to, KW_DEAL, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  cat_double_to_tstream(to, a.value.attack.magnitude);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, KW_BASE, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, KW_DAMAGE, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, KW_TO, MAX_TEXT_STREAM_LEN);
  strncat(to, KW_SPACE, MAX_TEXT_STREAM_LEN);

  strncat(to, target_to_string(a.target), MAX_TEXT_STREAM_LEN);

  strncat(to, KW_PERIOD, MAX_TEXT_STREAM_LEN);
}

void action_to_tstream_nl(char * to, Action a) {
  switch (a.type) {
    case ENEMY_DEBUFF:
      cat_enemy_debuff_to_tstream(to, a);
      break;
    case SELF_BUFF:
      cat_self_buff_to_tstream(to, a);
      break;
    case ATTACK:
      cat_attack_to_tstream(to, a);
      break;
  }
  strncat(to, KW_NL, MAX_TEXT_STREAM_LEN);
}

size_t max_tstream_len(TextStream tstream) {
  size_t max_len = 0;
  char * buf = NULL;
  do {
    if (!buf) {
      buf = strtok(tstream, KW_NL);
    } else {
      buf = strtok(NULL, KW_NL);
    }
    if (buf) {
      size_t len = strnlen(buf, MAX_TEXT_STREAM_LEN);
      if (len > max_len) {
        max_len = len;
      }
    }
  } while (buf);

  return max_len;
}

size_t max_tstream_height(TextStream tstream) {
  size_t height = 0;
  char * buf = NULL;
  do {
    if (!buf) {
      buf = strtok(tstream, KW_NL);
    } else {
      buf = strtok(NULL, KW_NL);
    }
    if (buf) {
      height++;
    }
  } while (buf);

  return height;
}

char * make_file_name(const char * root_dir, const char * file_name) {
  size_t root_dir_len = strnlen(root_dir, MAX_PATH_LEN);
  size_t file_name_len = strnlen(file_name, MAX_PATH_LEN);
  char * result = calloc(root_dir_len + file_name_len + 1, sizeof(char));
  strncpy(result, root_dir, root_dir_len);
  strncpy(result + root_dir_len, file_name, file_name_len);
  return result;
}

void dump_file_contents(TextStream tstream, const char * file_path) {
  FILE * fp = fopen(file_path, "r");
  struct stat st;
  fstat(fileno(fp), &st);

  fread(tstream, 1, st.st_size, fp);
  fclose(fp);
}

void card_to_stream(TextStream tstream, Card card) {
  strncat(tstream, card.name, MAX_CARD_NAME_LEN - 1);
  strncat(tstream, KW_NL, MAX_TEXT_STREAM_LEN);

  cat_double_to_tstream(tstream, card.cost);
  strncat(tstream, KW_NL, MAX_TEXT_STREAM_LEN);

  strncat(tstream, rarity_to_string(card.rarity), MAX_CARD_NAME_LEN - 1);
  strncat(tstream, KW_NL, MAX_TEXT_STREAM_LEN);

  for (uint8_t i = 0; i < card.qty_actions; i++) {
    action_to_tstream_nl(tstream, card.actions[i]);
  }
}

void hand_to_stream(TextStream tstream, CardPool card_pool) {
  for (uint8_t i = 0; i < card_pool.qty_cards; i++) {
    strncat(tstream, card_pool.cards[i].name, MAX_TEXT_STREAM_LEN);
    strncat(tstream, KW_NL, MAX_TEXT_STREAM_LEN);
  }
}

void move_to_stream(TextStream tstream, Move move) {
  for (uint8_t i = 0; i < move.qty_actions; i++) {
    action_to_tstream_nl(tstream, move.actions[i]);
  }
}
