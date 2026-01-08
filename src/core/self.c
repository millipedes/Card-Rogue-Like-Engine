#include "self.h"

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

Self init_self(const char * root_dir) {
  Self self = {0};
  char * tmp_art_file_name = make_file_name(root_dir, SELF_ART_FILE_NAME);
  self.art = dump_file_contents(tmp_art_file_name);
  free(tmp_art_file_name);

  char * tmp_card_pool_file_name = make_file_name(root_dir,
      SELF_CARD_POOL_FILE_NAME);
  char * card_pool_input = dump_file_contents(tmp_card_pool_file_name);
  parse_self_card_pool(card_pool_input, &self.card_pool);
  free(tmp_card_pool_file_name);
  free(card_pool_input);

  char * tmp_starting_deck_file_name = make_file_name(root_dir,
      SELF_STARTING_DECK_FILE_NAME);
  char * starting_deck_input = dump_file_contents(tmp_starting_deck_file_name);
  parse_self_starting_deck(starting_deck_input, &self.card_pool, &self.deck);
  free(tmp_starting_deck_file_name);
  free(starting_deck_input);

  return self;
}

void shuffle_deck(Deck deck) {
  for (uint8_t i = deck.qty_cards - 1; i > 0; i--) {
    uint8_t j = rand() % (i + 1);
    Card tmp = deck.cards[i];
    deck.cards[i] = deck.cards[j];
    deck.cards[j] = tmp;
  }
}

void free_self(Self self) {
  if (self.art) {
    free(self.art);
  }
  free_card_pool(self.card_pool);
  free_card_pool(self.deck);
}
