#include "self_state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SelfState init_self_state(const char * root_dir) {
  SelfState self_state = {0};
  char * tmp_art_file_name = make_file_name(root_dir, SELF_ART_FILE_NAME);
  dump_file_contents(self_state.art_lines, tmp_art_file_name);
  free(tmp_art_file_name);

  char * tmp_card_pool_file_name = make_file_name(root_dir,
      SELF_CARD_POOL_FILE_NAME);
  TextStream card_pool_input = {0};
  dump_file_contents(card_pool_input, tmp_card_pool_file_name);
  parse_self_card_pool(card_pool_input, &self_state.card_pool);
  free(tmp_card_pool_file_name);

  char * tmp_starting_deck_file_name = make_file_name(root_dir,
      SELF_STARTING_DECK_FILE_NAME);
  TextStream starting_deck_input = {0};
  dump_file_contents(starting_deck_input, tmp_starting_deck_file_name);
  parse_self_starting_deck(starting_deck_input, &self_state.card_pool,
      &self_state.deck);
  free(tmp_starting_deck_file_name);

  return self_state;
}

void free_self_state(SelfState self_state) {
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
