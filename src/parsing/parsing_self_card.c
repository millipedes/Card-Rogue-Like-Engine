#include "parsing_self_card.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const char * parse_attack(const char * input, CardAction * action) {
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

  *action = (CardAction){
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

const char * parse_self_buff(const char * input, CardAction * action) {
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

  *action = (CardAction){
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

const char * parse_enemy_debuff(const char * input, CardAction * action) {
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

  *action = (CardAction){
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

const char * parse_card_value(const char * input, CardAction * action) {
  const char * result = NULL;
  if (result = parse_attack(parse_ws(input), action)) {
    return result;
  } else if (result = parse_self_buff(parse_ws(input), action)) {
    return result;
  } else if (result = parse_enemy_debuff(parse_ws(input), action)) {
    return result;
  } else return NULL;
}

void add_card_action(Card * card, CardAction action) {
  if (!card->actions) {
    card->qty_actions = 1;
    card->actions = calloc(1, sizeof(CardAction));
  } else {
    card->qty_actions++;
    card->actions = realloc(card->actions,
        card->qty_actions * sizeof(CardAction));
  }
  card->actions[card->qty_actions - 1] = action;
}

const char * parse_card_actions(const char * input, Card * card) {
  const char * current_input = input;
  const char * final_input = NULL;

  CardAction tmp = {0};
  while (current_input = parse_card_value(parse_ws(current_input), &tmp)) {
    final_input = current_input;
    add_card_action(card, tmp);
    tmp = (CardAction){0};
  }
  return parse_ws(final_input);
}

const char * parse_rarity(const char * input, Rarity * rarity) {
  const char * result = NULL;
  if (result = parse_keyword(parse_ws(input), KW_COMMON)) {
    *rarity = COMMON;
  } else if (result = parse_keyword(parse_ws(input), KW_UNCOMMON)) {
    *rarity = UNCOMMON;
  } else if (result = parse_keyword(parse_ws(input), KW_RARE)) {
    *rarity = RARE;
  } else if (result = parse_keyword(parse_ws(input), KW_SUMM)) {
    *rarity = SUMM;
  }
  return result;
}

const char * parse_name(const char * input, Card * card) {
  const char * end_field = strchr(input, CHAR_VERT_BAR);
  if (!end_field) {
    return NULL;
  }

  uint16_t inc = 1;
  while (is_whitespace(*(end_field - inc))) inc++;

  // After the while loop, inc will be on the last character
  inc--;

  size_t name_len = end_field - input - inc;

  // no need to worry about NUL terminator with calloc/copy combo
  card->name = calloc(name_len + 1, sizeof(char));
  strncpy(card->name, input, name_len);

  return end_field;
}

const char * parse_card(const char * input, Card * card) {
  // see header description
  uint8_t attribute_bitmap = 0;

  const char * result = input;
  const char * tmp_result = NULL;
  // This nonsense is to allow order independence, easier for the user, any
  // questions, again see the header.
  while (attribute_bitmap != SELF_CARD_ATTRIBUTE_BIT_NO && result != NULL) {
    if (tmp_result = parse_keywords(parse_ws(result), 2, KW_NAME, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_name(parse_ws(tmp_result), card))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << NAME_BIT);
      result = tmp_result;

    } else if (tmp_result = parse_keywords(parse_ws(result), 2, KW_COST, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_number(parse_ws(tmp_result), &card->cost))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << COST_BIT);
      result = tmp_result;

    } else if (tmp_result = parse_keywords(parse_ws(result), 2, KW_RARITY, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_rarity(parse_ws(tmp_result), &card->rarity))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << RARITY_BIT);
      result = tmp_result;

    } else if (tmp_result = parse_keywords(parse_ws(result), 2, KW_ACTION, KW_COLON)) {
      if (!(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))
          || !(tmp_result = parse_card_actions(parse_ws(tmp_result), card))
          || !(tmp_result = parse_keyword(parse_ws(tmp_result), KW_VERT_BAR))) {
        return NULL;
      }
      attribute_bitmap |= (1U << ACTION_BIT);
      result = tmp_result;

    } else {
      result = NULL;
    }
  }
  return result;
}

void add_card(CardPool * card_pool, Card card) {
  if (!card_pool->qty_cards) {
    card_pool->qty_cards = 1;
    card_pool->cards = calloc(1, sizeof(Card));
  } else {
    card_pool->qty_cards++;
    card_pool->cards = realloc(card_pool->cards,
        card_pool->qty_cards * sizeof(Card));
  }
  card_pool->cards[card_pool->qty_cards - 1] = card;
}

bool find_card(const CardPool * card_pool, const char * name,
    uint8_t * index_on_success) {
  for (uint8_t i = 0; i < card_pool->qty_cards; i++) {
    if (!strncmp(name, card_pool->cards[i].name, MAX_CARD_NAME_LEN)) {
      *index_on_success = i;
      return true;
    }
  }
  return false;
}

Card deep_copy_card(const Card old) {
  Card copy = {0};
  size_t name_len = strnlen(old.name, MAX_CARD_NAME_LEN);
  copy.name = calloc(name_len + 1, sizeof(char));
  strncpy(copy.name, old.name, name_len);
  copy.cost = old.cost;
  copy.rarity = old.rarity;
  copy.qty_actions = old.qty_actions;
  copy.actions = calloc(copy.qty_actions, sizeof(CardAction));
  for (uint8_t i = 0; i < copy.qty_actions; i++) {
    copy.actions[i] = old.actions[i];
  }
  return copy;
}

const char * parse_self_card_pool(const char * input, CardPool * card_pool) {
  const char * current_input = input;
  const char * final_input = NULL;

  Card tmp = {0};
  while (current_input = parse_card(parse_ws(current_input), &tmp)) {
    final_input = current_input;
    add_card(card_pool, tmp);
    tmp = (Card){0};
  }
  return parse_ws(final_input);
}

const char * parse_self_starting_deck(const char * input,
    const CardPool * card_pool, Deck * deck) {
  const char * result = input;
  const char * tmp_result = NULL;

  while (true) {
    tmp_result = strchr(result, CHAR_COLON);
    if (!tmp_result) {
      break;
    }

    size_t name_len = tmp_result - result;
    char * name = calloc(name_len + 1, sizeof(char));
    strncpy(name, result, name_len);
    // We know it must exist as strchr did not return NULL, after the name stuff
    // as we do not want the name to have the ":"
    tmp_result = parse_keyword(parse_ws(tmp_result), KW_COLON);

    uint8_t card_pool_index = 0;
    if (!find_card(card_pool, name, &card_pool_index)) {
      free(name);
      return NULL;
    }

    double tmp = 0.0;
    if (!(tmp_result = parse_number(parse_ws(tmp_result), &tmp))) {
      free(name);
      return NULL;
    }

    for (int i = 0; i < (int)tmp; i++) {
      add_card(deck, deep_copy_card(card_pool->cards[card_pool_index]));
    }

    free(name);

    result = parse_ws(tmp_result);
  }

  return result;
}

void free_card(Card card) {
  if (card.name) {
    free(card.name);
  }

  if (card.actions) {
    free(card.actions);
  }
}

void free_card_pool(CardPool card_pool) {
  for (uint8_t i = 0; i < card_pool.qty_cards; i++) {
    free_card(card_pool.cards[i]);
  }
  if (card_pool.cards) {
    free(card_pool.cards);
  }
}
