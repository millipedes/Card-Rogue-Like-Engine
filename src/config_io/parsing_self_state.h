#ifndef PARSING_SELF_CARD_H
#define PARSING_SELF_CARD_H

#include <stdbool.h>
#include <stdint.h>

#include "parsing_common_state.h"
#include "parsing.h"

// Rarity Keywords
#define KW_COMMON   "Common"
#define KW_UNCOMMON "UnCommon"
#define KW_RARE     "Rare"
#define KW_SUMM     "Summ"

// Top-Level Self Specific Keywords
#define KW_NAME     "Name"
#define KW_COST     "Cost"
#define KW_ACTION   "Action"
#define KW_RARITY   "Rarity"
// These are for a parsing trick
#define CHAR_COLON    ':'
#define CHAR_VERT_BAR '|'

// Bitmap is as follows:
// (Action) (Rarity) (Cost) (Name)
// I.e.
//        1        1      1      1
// => Seen everything
//        0        0      0      0
// => Seen nothing
#define SELF_CARD_ATTRIBUTE_BIT_NO 0b1111
#define ACTION_BIT 3
#define RARITY_BIT 2
#define COST_BIT   1
#define NAME_BIT   0

#define MAX_CARD_NAME_LEN 128

typedef enum {
  COMMON,
  UNCOMMON,
  RARE,
  SUMM,
} Rarity;

const char * rarity_to_string(Rarity rarity);

typedef struct {
  char * name;
  double cost;
  Rarity rarity;
  Action * actions;
  uint8_t qty_actions;
} Card;

typedef struct {
  Card * cards;
  uint8_t qty_cards;
} CardPool;

typedef CardPool Deck;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

const char * parse_self_card_pool(const char * input, CardPool * card_pool);
const char * parse_self_starting_deck(const char * input,
    const CardPool * card_pool, Deck * deck);

void free_card(Card card);

// Note that these also work with the Deck type as Deck is a typedefed CardPool
void add_card(CardPool * card_pool, Card card);
bool find_card(const CardPool * card_pool, const char * name,
    uint8_t * index_on_success);
Card deep_copy_card(const Card old);
void free_card_pool(CardPool card_pool);

#ifdef __cplusplus
}
#endif

#endif
