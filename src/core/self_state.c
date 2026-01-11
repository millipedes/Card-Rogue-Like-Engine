#include "self_state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char * make_file_name(const char * root_dir, const char * file_name) {
  size_t root_dir_len = strnlen(root_dir, MAX_PATH_LEN);
  size_t file_name_len = strnlen(file_name, MAX_PATH_LEN);
  char * result = calloc(root_dir_len + file_name_len + 1, sizeof(char));
  strncpy(result, root_dir, root_dir_len);
  strncpy(result + root_dir_len, file_name, file_name_len);
  return result;
}

char * dump_file_contents(const char * file_path) {
  FILE * fp = fopen(file_path, "r");
  struct stat st;
  fstat(fileno(fp), &st);

  char * into = calloc(st.st_size + 1, sizeof(char));

  fread(into, 1, st.st_size, fp);
  fclose(fp);
  return into;
}

void add_art_line(SelfState * self_state, const char * new_line) {
  if (!self_state->art_lines) {
    self_state->qty_art_lines = 1;
    self_state->art_lines = calloc(1, sizeof(char *));
  } else {
    self_state->qty_art_lines++;
    self_state->art_lines = realloc(self_state->art_lines,
        self_state->qty_art_lines * sizeof(char *));
  }
  size_t len = strnlen(new_line, ART_MAX_WIDTH);
  self_state->art_lines[self_state->qty_art_lines - 1] = calloc(len + 1, sizeof(char));
  strncpy(self_state->art_lines[self_state->qty_art_lines - 1], new_line, len);
}

void get_art_lines(const char * file_path, SelfState * self_state) {
  FILE * fp = fopen(file_path, "r");
  struct stat st;
  fstat(fileno(fp), &st);

  char * into = calloc(st.st_size + 1, sizeof(char));

  fread(into, 1, st.st_size, fp);
  fclose(fp);

  char * tmp = NULL;
  do {
    if (!tmp) {
      tmp = strtok(into, NEWLINE_TOK);
    } else {
      tmp = strtok(NULL, NEWLINE_TOK);
    }
    if (tmp) {
      add_art_line(self_state, tmp);
    }
  } while (tmp);
  free(into);
}

SelfState init_self_state(const char * root_dir) {
  SelfState self_state = {0};
  char * tmp_art_file_name = make_file_name(root_dir, SELF_ART_FILE_NAME);
  get_art_lines(tmp_art_file_name, &self_state);
  free(tmp_art_file_name);

  char * tmp_card_pool_file_name = make_file_name(root_dir,
      SELF_CARD_POOL_FILE_NAME);
  char * card_pool_input = dump_file_contents(tmp_card_pool_file_name);
  parse_self_card_pool(card_pool_input, &self_state.card_pool);
  free(tmp_card_pool_file_name);
  free(card_pool_input);

  char * tmp_starting_deck_file_name = make_file_name(root_dir,
      SELF_STARTING_DECK_FILE_NAME);
  char * starting_deck_input = dump_file_contents(tmp_starting_deck_file_name);
  parse_self_starting_deck(starting_deck_input, &self_state.card_pool,
      &self_state.deck);
  free(tmp_starting_deck_file_name);
  free(starting_deck_input);

  return self_state;
}

size_t max_art_width(SelfState self_state) {
  size_t max_len = 0;
  for (uint8_t i = 0; i < self_state.qty_art_lines; i++) {
    size_t curr_len = strnlen(self_state.art_lines[i], ART_MAX_WIDTH);
    if (curr_len > max_len) {
      max_len = curr_len;
    }
  }
  return max_len;
}

size_t max_hand_name_width(SelfState self_state) {
  size_t max_len = 0;
  for (uint8_t i = 0; i < self_state.qty_hand; i++) {
    size_t curr_len = strnlen(self_state.hand[i]->name, MAX_CARD_NAME_LEN);
    if (curr_len > max_len) {
      max_len = curr_len;
    }
  }
  return max_len;
}

void free_self_state(SelfState self_state) {
  for (uint8_t i = 0; i < self_state.qty_art_lines; i++) {
    if (self_state.art_lines[i]) {
      free(self_state.art_lines[i]);
    }
  }
  if (self_state.art_lines) {
    free(self_state.art_lines);
  }
  free_card_pool(self_state.card_pool);
  free_card_pool(self_state.deck);
}

void reset_card_set(CardSetRef ref) {
  memset(ref, 0, sizeof(Card *) * MAX_SET_LEN);
}

SelfState start_battle(SelfState self_state, uint8_t starting_hand_size) {
  for (uint8_t i = 0; i < self_state.deck.qty_cards; i++) {
    self_state.in_deck[i] = &self_state.deck.cards[i];
  }
  self_state.qty_in_deck = self_state.deck.qty_cards;
  shuffle_deck(self_state.in_deck, self_state.qty_in_deck);

  self_state = draw_cards(self_state, starting_hand_size);

  return self_state;
}

SelfState draw_cards(SelfState self_state, uint8_t qty) {
  for (uint8_t i = 0; i < qty; i++) {
    if (self_state.qty_in_deck == 0) {
      if (self_state.qty_discard == 0) {
        break;
      }
      self_state = shuffle_discard_to_deck(self_state);
    }
    self_state.qty_in_deck--;
    self_state.hand[self_state.qty_hand] = self_state.in_deck[self_state.qty_in_deck];
    self_state.in_deck[self_state.qty_in_deck] = NULL;
    self_state.qty_hand++;
  }
  return self_state;
}

SelfState discard_hand(SelfState self_state) {
  self_state.qty_discard = self_state.qty_hand;
  for (uint8_t i = 0; i < self_state.qty_hand; i++) {
    self_state.discard[i] = self_state.hand[i];
  }
  reset_card_set(self_state.hand);
  self_state.qty_hand = 0;
  return self_state;
}

SelfState shuffle_discard_to_deck(SelfState self_state) {
  for (uint8_t i = 0; i < self_state.qty_discard; i++) {
    self_state.in_deck[self_state.qty_in_deck] = self_state.discard[i];
    self_state.qty_in_deck++;
  }
  reset_card_set(self_state.discard);
  self_state.qty_discard = 0;
  shuffle_deck(self_state.in_deck, self_state.qty_in_deck);
  return self_state;
}

void shuffle_deck(CardSetRef deck, uint8_t qty) {
  if (qty == 0) {
    return;
  }
  for (uint8_t i = qty - 1; i > 0; i--) {
    uint8_t j = rand() % (i + 1);
    Card * tmp = deck[i];
    deck[i] = deck[j];
    deck[j] = tmp;
  }
}

SelfState finish_battle(SelfState self_state) {
  reset_card_set(self_state.hand);
  self_state.qty_hand = 0;
  reset_card_set(self_state.discard);
  self_state.qty_discard = 0;
  reset_card_set(self_state.removed);
  self_state.qty_removed = 0;
  reset_card_set(self_state.in_deck);
  self_state.qty_in_deck = 0;
  return self_state;
}
