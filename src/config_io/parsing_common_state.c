#include "parsing_common_state.h"

#include <stdio.h>
#include <stdlib.h>

const char * enemy_debuff_type_to_string(EnemyDebuffType type) {
  switch (type) {
    case STUMBLE: return "Stumble";
    case CORRODE: return "Corrode";
  }
  return NULL;
}

const char * self_buff_type_to_string(SelfBuffType type) {
  switch (type) {
    case DAMAGE:  return "Damage";
    case DEFENSE: return "Defense";
  }
  return NULL;
}

const char * target_to_string(Target target) {
  switch (target) {
    case SELF:         return "Self";
    case TARGET_ENEMY: return "Target Enemy";
    case ALL_ENEMIES:  return "All Enemies";
  }
  return NULL;
}

const char * parse_target(const char * input, Target * target) {
  const char * result = NULL;
  if (result = parse_keywords(parse_ws(input), 2, KW_TARGET, KW_ENEMY)) {
    *target = TARGET_ENEMY;
    return result;
  } else if (result = parse_keywords(parse_ws(input), 2, KW_ALL, KW_ENEMIES)) {
    *target = ALL_ENEMIES;
    return result;
  } else return NULL;
}

const char * parse_attack(const char * input, Action * action) {
  const char * result = NULL;
  double magnitude = 0.0;
  Target target;
  if (!(result = parse_keyword(parse_ws(input), KW_DEAL))
      || !(result = parse_number(parse_ws(result), &magnitude))
      || !(result = parse_keywords(parse_ws(result), 3, KW_BASE, KW_DAMAGE, KW_TO))
      || !(result = parse_target(parse_ws(result), &target))
      || !(result = parse_keyword(parse_ws(result), KW_PERIOD))) {
    return NULL;
  }

  *action = (Action){
    .value = (EffectValue){
      .attack = (Attack){ .magnitude = magnitude },
    },
    .type = ATTACK,
    .target = target,
  };
  return result;
}

const char * parse_self_buff_type(const char * input, SelfBuffType * type) {
  const char * result = NULL;
  if ((result = parse_keyword(parse_ws(input), KW_DAMAGE))) {
    *type = DAMAGE;
    return result;
  } else if (result = parse_keyword(parse_ws(input), KW_DEFENSE)) {
    *type = DEFENSE;
    return result;
  } else return NULL;
}

const char * parse_duration(const char * input, Duration * duration) {
  const char * result = NULL;
  if (!(result = parse_keyword(parse_ws(input), KW_FOR))) {
    // No duration implies indifnitely
    *duration = (Duration){
      .value = (DurationValue){ .indefinite = true },
      .type = INDEFINITE,
    };
    return input;
  }

  double tmp = 0.0;
  if (!(result = parse_number(parse_ws(result), &tmp))) {
    return NULL;
  }

  if (tmp <= 0.0) {
    fprintf(stderr, "parse_duration: negative/0 turn number passed: `%f`\n", tmp);
    exit(1);
  }

  if (tmp > 255.0) {
    fprintf(stderr, "parse_duration: max turn number is 255: `%f`\n", tmp);
    exit(1);
  }

  if (tmp == 1) {
    if (!(result = parse_keyword(parse_ws(result), KW_TURN))) {
      return NULL;
    }
  } else {
    if (!(result = parse_keyword(parse_ws(result), KW_TURNS))) {
      return NULL;
    }
  }

  *duration = (Duration){
    .value = (DurationValue){ .remaining_turns = (uint8_t)tmp },
    .type = FINITE,
  };
  return result;
}

const char * parse_self_buff(const char * input, Action * action) {
  const char * result = NULL;
  double magnitude = 0.0;
  SelfBuffType type;
  Duration duration = {0};
  if (!(result = parse_keyword(parse_ws(input), KW_GAIN))
      || !(result = parse_number(parse_ws(result), &magnitude))
      || !(result = parse_self_buff_type(parse_ws(result), &type))
      || !(result = parse_duration(parse_ws(result), &duration))
      || !(result = parse_keyword(parse_ws(result), KW_PERIOD))) {
    return NULL;
  }

  *action = (Action){
    .value = (EffectValue){
      .self_buff = (SelfBuff){
        .duration = duration,
        .magnitude = magnitude,
        .type = type,
      },
    },
    .type = SELF_BUFF,
    .target = SELF,
  };
  return result;
}

const char * parse_enemy_debuff_type(const char * input, EnemyDebuffType * type) {
  const char * result = NULL;
  if ((result = parse_keyword(parse_ws(input), KW_STUMBLE))) {
    *type = STUMBLE;
    return result;
  } else if (result = parse_keyword(parse_ws(input), KW_CORRODE)) {
    *type = CORRODE;
    return result;
  } else return NULL;
}

const char * parse_enemy_debuff(const char * input, Action * action) {
  const char * result = NULL;
  double magnitude = 0.0;
  EnemyDebuffType type;
  Target target;
  Duration duration = {0};
  if (!(result = parse_keyword(parse_ws(input), KW_APPLY))
      || !(result = parse_number(parse_ws(result), &magnitude))
      || !(result = parse_enemy_debuff_type(parse_ws(result), &type))
      || !(result = parse_keyword(parse_ws(result), KW_TO))
      || !(result = parse_target(parse_ws(result), &target))
      || !(result = parse_duration(parse_ws(result), &duration))
      || !(result = parse_keyword(parse_ws(result), KW_PERIOD))) {
    return NULL;
  }

  *action = (Action){
    .value = (EffectValue){
      .enemy_debuff = (EnemyDebuff){
        .duration = duration,
        .magnitude = magnitude,
        .type = type,
      },
    },
    .type = ENEMY_DEBUFF,
    .target = target,
  };
  return result;
}

const char * parse_action(const char * input, Action * action) {
  const char * result = NULL;
  if (result = parse_attack(parse_ws(input), action)) {
    return result;
  } else if (result = parse_self_buff(parse_ws(input), action)) {
    return result;
  } else if (result = parse_enemy_debuff(parse_ws(input), action)) {
    return result;
  } else return NULL;
}
