#ifndef COMMON_STATE_PARSING_H
#define COMMON_STATE_PARSING_H

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

// Common Top-Level Keywords
#define KW_COLON    ":"
#define KW_VERT_BAR "|"

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

const char * enemy_debuff_type_to_string(EnemyDebuffType type);

typedef struct {
  Duration duration;
  double magnitude;
  EnemyDebuffType type;
} EnemyDebuff;

typedef enum {
  DAMAGE,
  DEFENSE,
} SelfBuffType;

const char * self_buff_type_to_string(SelfBuffType type);

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

typedef enum {
  SELF,
  TARGET_ENEMY,
  ALL_ENEMIES,
} Target;

const char * target_to_string(Target target);

typedef struct {
  EffectValue value;
  EffectType type;
  Target target;
} Action;

const char * parse_action(const char * input, Action * action);

#endif
