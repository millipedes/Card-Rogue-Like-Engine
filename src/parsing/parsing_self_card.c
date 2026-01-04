#include "parsing_self_card.h"

#include <stdio.h>
#include <stdlib.h>

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

const char * parse_attack(const char * input, CardValue * value) {
  const char * result = NULL;
  if (!(result = parse_keyword(parse_ws(input), KW_DEAL))) {
    return NULL;
  }

  double magnitude = 0.0;
  if (!(result = parse_number(parse_ws(result), &magnitude))) {
    return NULL;
  }

  if (!(result = parse_keywords(parse_ws(result), 3, KW_BASE, KW_DAMAGE, KW_TO))) {
    return NULL;
  }

  Target target;
  if (!(result = parse_target(parse_ws(result), &target))) {
    return NULL;
  }

  if (!(result = parse_keyword(parse_ws(result), KW_PERIOD))) {
    return NULL;
  }

  *value = (CardValue){
    .effect = (Effect){
      .value = (EffectValue){
        .attack = (Attack){ .magnitude = magnitude },
      },
      .type = ATTACK,
    },
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

const char * parse_self_buff(const char * input, CardValue * value) {
  const char * result = NULL;
  if (!(result = parse_keyword(parse_ws(input), KW_GAIN))) {
    return NULL;
  }

  double magnitude = 0.0;
  if (!(result = parse_number(parse_ws(result), &magnitude))) {
    return NULL;
  }

  SelfBuffType type;
  if (!(result = parse_self_buff_type(parse_ws(result), &type))) {
    return NULL;
  }

  Duration duration = {0};
  if (!(result = parse_duration(parse_ws(result), &duration))) {
    return NULL;
  }

  if (!(result = parse_keyword(parse_ws(result), KW_PERIOD))) {
    return NULL;
  }

  *value = (CardValue){
    .effect = (Effect){
      .value = (EffectValue){
        .self_buff = (SelfBuff){
          .duration = duration,
          .magnitude = magnitude,
          .type = type,
        },
      },
      .type = SELF_BUFF,
    },
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

const char * parse_enemy_debuff(const char * input, CardValue * value) {
  const char * result = NULL;
  if (!(result = parse_keyword(parse_ws(input), KW_APPLY))) {
    return NULL;
  }

  double magnitude = 0.0;
  if (!(result = parse_number(parse_ws(result), &magnitude))) {
    return NULL;
  }

  EnemyDebuffType type;
  if (!(result = parse_enemy_debuff_type(parse_ws(result), &type))) {
    return NULL;
  }

  if (!(result = parse_keyword(parse_ws(result), KW_TO))) {
    return NULL;
  }

  Target target;
  if (!(result = parse_target(parse_ws(result), &target))) {
    return NULL;
  }

  Duration duration = {0};
  if (!(result = parse_duration(parse_ws(result), &duration))) {
    return NULL;
  }

  if (!(result = parse_keyword(parse_ws(result), KW_PERIOD))) {
    return NULL;
  }

  *value = (CardValue){
    .effect = (Effect){
      .value = (EffectValue){
        .enemy_debuff = (EnemyDebuff){
          .duration = duration,
          .magnitude = magnitude,
          .type = type,
        },
      },
      .type = ENEMY_DEBUFF,
    },
    .target = target,
  };
  return result;
}

const char * parse_card_value(const char * input, CardValue * value) {
  const char * result = NULL;
  if (result = parse_attack(parse_ws(input), value)) {
    return result;
  } else if (result = parse_self_buff(parse_ws(input), value)) {
    return result;
  } else if (result = parse_enemy_debuff(parse_ws(input), value)) {
    return result;
  } else return NULL;
}

void add_card_value(Action * action, CardValue value) {
  if (!action->values) {
    action->qty_values = 1;
    action->values = calloc(1, sizeof(CardValue));
  } else {
    action->qty_values++;
    action->values = realloc(action->values,
        action->qty_values * sizeof(CardValue));
  }
  action->values[action->qty_values - 1] = value;
}

const char * parse_card_actions(const char * input, Action * action) {
  const char * current_input = input;
  const char * final_input = NULL;

  CardValue tmp = {0};
  while (current_input = parse_card_value(parse_ws(current_input), &tmp)) {
    final_input = current_input;
    add_card_value(action, tmp);
    tmp = (CardValue){0};
  }
  return parse_ws(final_input);
}

void free_action(Action action) {
  if (action.values) {
    free(action.values);
  }
}

