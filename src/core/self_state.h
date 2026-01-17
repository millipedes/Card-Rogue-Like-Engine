#ifndef SELF_STATE_H
#define SELF_STATE_H

#include <stddef.h>

#include "config_io/parsing_self_state.h"

#define SELF_ART_FILE_NAME           "art.txt"
#define SELF_CARD_POOL_FILE_NAME     "card_pool.txt"
#define SELF_STARTING_DECK_FILE_NAME "starting_deck.txt"

#define MAX_SET_LEN 128

#define STARTING_HAND_SIZE 7

typedef Card * CardSetRef[MAX_SET_LEN];

typedef struct {
  char ** art_lines; // pre-grabbing lines leads to less friction with ncurses
  uint8_t qty_art_lines;
  Deck deck;
  CardPool card_pool;

  CardSetRef hand;
  uint8_t qty_hand;
  CardSetRef discard;
  uint8_t qty_discard;
  CardSetRef removed;
  uint8_t qty_removed;
  // 0           -> bottom of deck
  // qty_in_deck -> top of deck
  CardSetRef in_deck;
  uint8_t qty_in_deck;

} SelfState;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

// SelfState Maintenance
SelfState init_self_state(const char * root_dir);
size_t max_hand_name_width(SelfState self_state);
void free_self_state(SelfState self);

// Deck Operations
SelfState start_battle(SelfState self_state, uint8_t starting_hand_size);
SelfState draw_cards(SelfState self_state, uint8_t qty);
SelfState discard_hand(SelfState self_state);
SelfState shuffle_discard_to_deck(SelfState self_state);
void shuffle_deck(CardSetRef deck, uint8_t qty);
SelfState finish_battle(SelfState self_state);

#ifdef __cplusplus
}
#endif

#endif
