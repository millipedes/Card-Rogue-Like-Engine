#ifndef PARSING_SELF_CARD_H
#define PARSING_SELF_CARD_H

#include <stdbool.h>
#include <stdint.h>

#include "parsing.h"

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

typedef struct {
  CardAction * actions;
  uint8_t qty_actions;
} Card;

// Public API
#ifdef __cplusplus
extern "C" {
#endif

const char * parse_card_actions(const char * input, Card * card);

void free_card(Card card);

#ifdef __cplusplus
}
#endif

#endif
