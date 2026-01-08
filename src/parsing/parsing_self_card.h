#ifndef PARSING_SELF_CARD_H
#define PARSING_SELF_CARD_H

#include <stdbool.h>
#include <stdint.h>

#include "parsing.h"

// Action Keywords
#define KW_TARGET  "Target"
#define KW_ENEMY   "Enemy"
#define KW_ENEMIES "Enemies"
#define KW_ALL     "All"
#define KW_DEAL    "Deal"
#define KW_BASE    "Base"
#define KW_TO      "to"
#define KW_GAIN    "Gain"
#define KW_DEFENSE "Defense"
#define KW_PERIOD  "."
#define KW_FOR     "for"
#define KW_DAMAGE  "Damage"
#define KW_APPLY   "Apply"
#define KW_CORRODE "Corrode"
#define KW_STUMBLE "Stumble"
#define KW_TURNS   "Turns"
#define KW_TURN    "Turn"

// Rarity Keywords
#define KW_COMMON   "Common"
#define KW_UNCOMMON "UnCommon"
#define KW_RARE     "Rare"
#define KW_SUMM     "Summ"

// Top Level Keywords
#define KW_NAME     "Name"
#define KW_COST     "Cost"
#define KW_ACTION   "Action"
#define KW_RARITY   "Rarity"
#define KW_COLON    ":"
#define KW_VERT_BAR "|"
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
  INDEFINITE,
  FINITE,
} DurationType;

typedef union {
  bool indefinite;
  uint8_t remaining_turns;
} DurationValue;

typedef struct {
  DurationValue value;
  DurationType type;
} Duration;

typedef enum {
  STUMBLE,
  CORRODE,
} EnemyDebuffType;

typedef struct {
  Duration duration;
  double magnitude;
  EnemyDebuffType type;
} EnemyDebuff;

typedef enum {
  DAMAGE,
  DEFENSE,
} SelfBuffType;

typedef struct {
  Duration duration;
  double magnitude;
  SelfBuffType type;
} SelfBuff;

typedef struct {
  double magnitude;
} Attack;

typedef enum {
  ENEMY_DEBUFF,
  SELF_BUFF,
  ATTACK,
} EffectType;

typedef union {
  EnemyDebuff enemy_debuff;
  SelfBuff self_buff;
  Attack attack;
} EffectValue;

typedef struct {
  EffectValue value;
  EffectType type;
} Effect;

typedef enum {
  SELF,
  TARGET_ENEMY,
  ALL_ENEMIES,
} Target;

typedef struct {
  Effect effect;
  Target target;
} CardAction;

typedef enum {
  COMMON,
  UNCOMMON,
  RARE,
  SUMM,
} Rarity;

typedef struct {
  char * name;
  double cost;
  Rarity rarity;
  CardAction * actions;
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
