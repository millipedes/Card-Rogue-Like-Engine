#include <gtest/gtest.h>

#include <iostream>

#include "config_io/parsing.h"
#include "config_io/parsing_common_state.h"
#include "config_io/parsing_self_state.h"
#include "config_io/parsing_enemy_state.h"

#include "config_io/stream_self_card.h"

constexpr auto eps = 1e-6;

auto number_test_helper(double expected, const char * input) -> void {
  auto n = 0.0;

  input = parse_number(input, &n);
  ASSERT_EQ(input[0], '\0');

  ASSERT_NEAR(n, expected, eps);
}

TEST(number, number_0) {
  number_test_helper(2342, "2342");
}

TEST(number, number_1) {
  number_test_helper(-2342, "-2342");
}

TEST(number, number_2) {
  number_test_helper(2342, "+2342");
}

TEST(number, number_3) {
  number_test_helper(2342, "2342.");
}

TEST(number, number_4) {
  number_test_helper(-2342, "-2342.");
}

TEST(number, number_5) {
  number_test_helper(2342, "+2342.");
}

// Note this handles the non-empty case for both:
// | Sign? [0-9]+ "." [0-9]*
// | Sign? [0-9]* "." [0-9]+
// Hence only one test here, similarly for the exponent case.
TEST(number, number_6) {
  number_test_helper(2342.1, "2342.1");
}

TEST(number, number_7) {
  number_test_helper(-2342.1, "-2342.1");
}

TEST(number, number_8) {
  number_test_helper(2342.1, "+2342.1");
}

TEST(number, number_9) {
  number_test_helper(0.2342, ".2342");
}

TEST(number, number_10) {
  number_test_helper(-0.2342, "-.2342");
}

TEST(number, number_11) {
  number_test_helper(0.2342, "+.2342");
}

TEST(number, number_12) {
  number_test_helper(1e2, "1e2");
}

TEST(number, number_13) {
  number_test_helper(-1e2, "-1e2");
}

TEST(number, number_14) {
  number_test_helper(1e2, "+1e2");
}

TEST(number, number_15) {
  number_test_helper(1e-2, "1e-2");
}

TEST(number, number_16) {
  number_test_helper(-1e-2, "-1e-2");
}

TEST(number, number_17) {
  number_test_helper(1e-2, "+1e-2");
}

TEST(number, number_18) {
  number_test_helper(1e+2, "1e+2");
}

TEST(number, number_19) {
  number_test_helper(-1e+2, "-1e+2");
}

TEST(number, number_20) {
  number_test_helper(1e+2, "+1e+2");
}

TEST(number, number_21) {
  number_test_helper(1e2, "1.e2");
}

TEST(number, number_22) {
  number_test_helper(1e-2, "1.e-2");
}

TEST(number, number_23) {
  number_test_helper(1e+2, "1.e+2");
}

TEST(number, number_24) {
  number_test_helper(-1e2, "-1.e2");
}

TEST(number, number_25) {
  number_test_helper(-1e-2, "-1.e-2");
}

TEST(number, number_26) {
  number_test_helper(-1e+2, "-1.e+2");
}

TEST(number, number_27) {
  number_test_helper(1e2, "+1.e2");
}

// see above comment
TEST(number, number_28) {
  number_test_helper(1.1e2, "1.1e2");
}

TEST(number, number_29) {
  number_test_helper(-1.1e2, "-1.1e2");
}

TEST(number, number_30) {
  number_test_helper(-1.1e-2, "-1.1e-2");
}

TEST(number, number_31) {
  number_test_helper(-1.1e+2, "-1.1e+2");
}

TEST(number, number_32) {
  number_test_helper(1.1e2, "+1.1e2");
}

TEST(number, number_33) {
  number_test_helper(1.1e-2, "+1.1e-2");
}

TEST(number, number_34) {
  number_test_helper(1.1e+2, "+1.1e+2");
}

TEST(number, number_35) {
  number_test_helper(.1e2, ".1e2");
}

TEST(number, number_36) {
  number_test_helper(.1e-2, ".1e-2");
}

TEST(number, number_37) {
  number_test_helper(.1e+2, ".1e+2");
}

TEST(number, number_38) {
  number_test_helper(-.1e2, "-.1e2");
}

TEST(number, number_39) {
  number_test_helper(-.1e-2, "-.1e-2");
}

TEST(number, number_40) {
  number_test_helper(-.1e+2, "-.1e+2");
}

TEST(number, number_41) {
  number_test_helper(.1e2, "+.1e2");
}

extern "C" const char * parse_attack(const char * input, Action * action);

TEST(attack, attack_0) {
  Action action = {0};
  auto result = parse_attack("Deal 3.2 Base Damage to All Enemies.", &action);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.attack.magnitude, 3.2, eps);
  ASSERT_EQ(action.type, ATTACK);
  ASSERT_EQ(action.target, ALL_ENEMIES);
}

TEST(attack, attack_1) {
  Action action = {0};
  auto result = parse_attack("Deal 3.2e-1 Base Damage to Target Enemy.", &action);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.attack.magnitude, 3.2e-1, eps);
  ASSERT_EQ(action.type, ATTACK);
  ASSERT_EQ(action.target, TARGET_ENEMY);
}

extern "C" const char * parse_self_buff(const char * input, Action * action);

TEST(self_buff, self_buff_0) {
  Action action = {0};
  auto result = parse_self_buff("Gain 3.2e-1 Defense.", &action);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.self_buff.magnitude, 3.2e-1, eps);
  ASSERT_EQ(action.value.self_buff.duration.type, INDEFINITE);
  ASSERT_EQ(action.value.self_buff.duration.value.indefinite, true);
  ASSERT_EQ(action.value.self_buff.type, DEFENSE);
  ASSERT_EQ(action.type, SELF_BUFF);

  ASSERT_EQ(action.target, SELF);
}

TEST(self_buff, self_buff_1) {
  Action action = {0};
  auto result = parse_self_buff("Gain 3.2e-1 Defense for 1 Turn.", &action);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.self_buff.magnitude, 3.2e-1, eps);
  ASSERT_EQ(action.value.self_buff.duration.type, FINITE);
  ASSERT_EQ(action.value.self_buff.duration.value.remaining_turns, 1);
  ASSERT_EQ(action.value.self_buff.type, DEFENSE);
  ASSERT_EQ(action.type, SELF_BUFF);

  ASSERT_EQ(action.target, SELF);
}

TEST(self_buff, self_buff_2) {
  Action action = {0};
  auto result = parse_self_buff("Gain 3.2e-1 Defense for 3 Turns.", &action);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.self_buff.magnitude, 3.2e-1, eps);
  ASSERT_EQ(action.value.self_buff.duration.type, FINITE);
  ASSERT_EQ(action.value.self_buff.duration.value.remaining_turns, 3);
  ASSERT_EQ(action.value.self_buff.type, DEFENSE);
  ASSERT_EQ(action.type, SELF_BUFF);

  ASSERT_EQ(action.target, SELF);
}

extern "C" const char * parse_enemy_debuff(const char * input, Action * action);

TEST(enemy_debuff, enemy_debuff_0) {
  Action action = {0};
  auto result = parse_enemy_debuff("Apply 0.35 Stumble to All Enemies.", &action);

  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(action.value.enemy_debuff.duration.type, INDEFINITE);
  ASSERT_EQ(action.value.enemy_debuff.duration.value.indefinite, true);
  ASSERT_EQ(action.value.enemy_debuff.type, STUMBLE);
  ASSERT_EQ(action.type, ENEMY_DEBUFF);

  ASSERT_EQ(action.target, ALL_ENEMIES);
}

TEST(enemy_debuff, enemy_debuff_1) {
  Action action = {0};
  auto result = parse_enemy_debuff("Apply 0.35 Corrode to Target Enemy for 1 Turn.", &action);

  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(action.value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(action.value.enemy_debuff.duration.type, FINITE);
  ASSERT_EQ(action.value.enemy_debuff.duration.value.remaining_turns, 1);
  ASSERT_EQ(action.value.enemy_debuff.type, CORRODE);
  ASSERT_EQ(action.type, ENEMY_DEBUFF);

  ASSERT_EQ(action.target, TARGET_ENEMY);
}

extern "C" const char * parse_card_actions(const char * input, Card * card);

TEST(card_action, card_action_0) {
  Card card = {0};
  auto input = R"(
# This is a really broken card :)
Deal 9.1 Base Damage to Target Enemy.
Deal 6.7 Base Damage to All Enemies.
Gain 100 Damage. Gain 40 Defense for 1 Turn. Gain 95 Damage for 2 Turns.
Apply 0.35 Corrode to All Enemies.
Apply 0.95 Stumble to All Enemies for 100 Turns.
Apply 1.00 Corrode to Target Enemy for 1e1 Turns.
)";
  auto result = parse_card_actions(input, &card);
  ASSERT_EQ(result[0], '\0');

  ASSERT_EQ(card.qty_actions, 8);

  // Attack Actions
  ASSERT_NEAR(card.actions[0].value.attack.magnitude, 9.1, eps);
  ASSERT_EQ(card.actions[0].type, ATTACK);
  ASSERT_EQ(card.actions[0].target, TARGET_ENEMY);
  ASSERT_NEAR(card.actions[1].value.attack.magnitude, 6.7, eps);
  ASSERT_EQ(card.actions[1].type, ATTACK);
  ASSERT_EQ(card.actions[1].target, ALL_ENEMIES);

  // Self Buff Actions
  ASSERT_NEAR(card.actions[2].value.self_buff.magnitude, 1e2, eps);
  ASSERT_EQ(card.actions[2].value.self_buff.duration.type, INDEFINITE);
  ASSERT_EQ(card.actions[2].value.self_buff.duration.value.indefinite, true);
  ASSERT_EQ(card.actions[2].value.self_buff.type, DAMAGE);
  ASSERT_EQ(card.actions[2].type, SELF_BUFF);
  ASSERT_EQ(card.actions[2].target, SELF);

  ASSERT_NEAR(card.actions[3].value.self_buff.magnitude, 4e1, eps);
  ASSERT_EQ(card.actions[3].value.self_buff.duration.type, FINITE);
  ASSERT_EQ(card.actions[3].value.self_buff.duration.value.remaining_turns, 1);
  ASSERT_EQ(card.actions[3].value.self_buff.type, DEFENSE);
  ASSERT_EQ(card.actions[3].type, SELF_BUFF);
  ASSERT_EQ(card.actions[3].target, SELF);

  ASSERT_NEAR(card.actions[4].value.self_buff.magnitude, 95, eps);
  ASSERT_EQ(card.actions[4].value.self_buff.duration.type, FINITE);
  ASSERT_EQ(card.actions[4].value.self_buff.duration.value.remaining_turns, 2);
  ASSERT_EQ(card.actions[4].value.self_buff.type, DAMAGE);
  ASSERT_EQ(card.actions[4].type, SELF_BUFF);
  ASSERT_EQ(card.actions[4].target, SELF);

  // Enemy Debuff Actions
  ASSERT_NEAR(card.actions[5].value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(card.actions[5].value.enemy_debuff.duration.type, INDEFINITE);
  ASSERT_EQ(card.actions[5].value.enemy_debuff.duration.value.indefinite, true);
  ASSERT_EQ(card.actions[5].value.enemy_debuff.type, CORRODE);
  ASSERT_EQ(card.actions[5].type, ENEMY_DEBUFF);
  ASSERT_EQ(card.actions[5].target, ALL_ENEMIES);

  ASSERT_NEAR(card.actions[6].value.enemy_debuff.magnitude, 9.5e-1, eps);
  ASSERT_EQ(card.actions[6].value.enemy_debuff.duration.type, FINITE);
  ASSERT_EQ(card.actions[6].value.enemy_debuff.duration.value.remaining_turns, 100);
  ASSERT_EQ(card.actions[6].value.enemy_debuff.type, STUMBLE);
  ASSERT_EQ(card.actions[6].type, ENEMY_DEBUFF);
  ASSERT_EQ(card.actions[6].target, ALL_ENEMIES);

  ASSERT_NEAR(card.actions[7].value.enemy_debuff.magnitude, 1e0, eps);
  ASSERT_EQ(card.actions[7].value.enemy_debuff.duration.type, FINITE);
  ASSERT_EQ(card.actions[7].value.enemy_debuff.duration.value.remaining_turns, 10);
  ASSERT_EQ(card.actions[7].value.enemy_debuff.type, CORRODE);
  ASSERT_EQ(card.actions[7].type, ENEMY_DEBUFF);
  ASSERT_EQ(card.actions[7].target, TARGET_ENEMY);

  free_card(card);
}

extern "C" const char * parse_name(const char * input, Card * card);

TEST(card_name, card_name_0) {
  // name inputs will be expected to be of this form without the opening
  // verticle bar. A little hacky, but imo the alternative is more hacky.
  auto input = R"(Scratch                             |)";
  Card card = {0};

  auto result = parse_name(input, &card);
  ASSERT_EQ(result[0], '|');
  ASSERT_STREQ(card.name, "Scratch");

  free_card(card);
}

TEST(card_name, card_name_1) {
  auto input = R"(Bad Luck Kitty                                 |)";
  Card card = {0};

  auto result = parse_name(input, &card);
  ASSERT_EQ(result[0], '|');
  ASSERT_STREQ(card.name, "Bad Luck Kitty");

  free_card(card);
}

TEST(card_name, card_name_2) {
  auto input = R"(Bad Luck Kitty                                 
|)";
  Card card = {0};

  auto result = parse_name(input, &card);
  ASSERT_EQ(result[0], '|');
  ASSERT_STREQ(card.name, "Bad Luck Kitty");

  free_card(card);
}

extern "C" const char * parse_card(const char * input, Card * card);

TEST(card, card_0) {
  auto input = R"(
# This is a cool card :)
Name:   | Bite   |
Cost:   | 2      |
Rarity: | Common |
Action: |
Deal 4 Base Damage to Target Enemy.
Apply 0.35 Stumble to Target Enemy.
|)";

  Card card = {0};
  auto result = parse_card(input, &card);

  ASSERT_STREQ(card.name, "Bite");
  ASSERT_NEAR(card.cost, 2, eps);
  ASSERT_EQ(card.rarity, COMMON);

  ASSERT_NEAR(card.actions[0].value.attack.magnitude, 4, eps);
  ASSERT_EQ(card.actions[0].type, ATTACK);
  ASSERT_EQ(card.actions[0].target, TARGET_ENEMY);

  ASSERT_NEAR(card.actions[1].value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(card.actions[1].value.enemy_debuff.duration.type, INDEFINITE);
  ASSERT_EQ(card.actions[1].value.enemy_debuff.duration.value.indefinite, true);
  ASSERT_EQ(card.actions[1].value.enemy_debuff.type, STUMBLE);
  ASSERT_EQ(card.actions[1].type, ENEMY_DEBUFF);
  ASSERT_EQ(card.actions[1].target, TARGET_ENEMY);

  ASSERT_EQ(card.qty_actions, 2);

  ASSERT_EQ(result[0], '\0');

  free_card(card);
}

// Given the above tests, I think that successfully parsing my first example
// input file and having the correct number of cards in the card pool is a
// sufficient final test.
TEST(self_card_set, self_card_set_0) {
  auto input = R"(Name:   | Scratch                             |
Cost:   | 1                                   |
Rarity: | Common                              |
Action: | Deal 3 Base Damage to Target Enemy. |

Name:   | Hidey Hole                 |
Cost:   | 1                          |
Rarity: | Common                     |
Action: | Gain 2 Defense for 1 Turn. |

Name:   | Bite   |
Cost:   | 2      |
Rarity: | Common |
Action: |
Deal 4 Base Damage to Target Enemy.
Apply 0.35 Stumble to Target Enemy.
|

Name:   | Bad Luck Kitty                                 |
Cost:   | 1                                              |
Rarity: | Common                                         |
Action: | Apply 0.65 Corrode to All Enemies for 2 Turns. |

Name:   | Zoomies        |
Cost:   | 1              |
Rarity: | UnCommon       |
Action: | Gain 1 Damage. |

Name:   | Sharp Claws                          |
Cost:   | 1                                    |
Rarity: | UnCommon                             |
Action: | Deal 15 Base Damage to Target Enemy. |

Name:   | Murder Mittens |
Cost:   | 2              |
Rarity: | UnCommon       |
Action: | Deal 20 Base Damage to Target Enemy. |

# Name:   | Laser Pointer Fury |
# Cost:   | 2                  |
# Rarity: | |
# Action: | |

# Name:   | All Paws on Deck |
# Cost:   | 1                |
# Rarity: | UnCommon         |
# Action: | Draw 3 Cards.    |

Name:   | Cat Nap                     |
Cost:   | 2                           |
Rarity: | Rare                        |
Action: | Gain 4 Defense for 3 Turns. |

Name:   | Cataclysm                            |
Cost:   | 3                                    |
Rarity: | Rare                                 |
Action: | Deal 40 Base Damage to Target Enemy. |
)";

  CardPool card_pool = {0};
  auto result = parse_self_card_pool(input, &card_pool);
  ASSERT_EQ(card_pool.qty_cards, 9);
  free_card_pool(card_pool);
  ASSERT_EQ(result[0], '\0');
}

TEST(starting_deck, starting_deck_0) {
  auto input_card_pool = R"(Name:   | Scratch                             |
Cost:   | 1                                   |
Rarity: | Common                              |
Action: | Deal 3 Base Damage to Target Enemy. |

Name:   | Hidey Hole                 |
Cost:   | 1                          |
Rarity: | Common                     |
Action: | Gain 2 Defense for 1 Turn. |)";

  CardPool card_pool = {0};
  auto result_card_pool = parse_self_card_pool(input_card_pool, &card_pool);
  ASSERT_EQ(card_pool.qty_cards, 2);
  ASSERT_EQ(result_card_pool[0], '\0');

  auto input_starting_deck = R"(Scratch: 5
Hidey Hole: 5)";

  Deck starting_deck = {0};
  auto result_starting_deck = parse_self_starting_deck(input_starting_deck,
      &card_pool, &starting_deck);
  ASSERT_EQ(starting_deck.qty_cards, 10);
  ASSERT_EQ(result_starting_deck[0], '\0');

  free_card_pool(card_pool);
  free_card_pool(starting_deck);
}

TEST(enemy_move_pool, enemy_move_pool_0) {
  auto input_move_pool = R"(Action:      | Deal 2 Base Damage to All Enemies. |
Cardinality: | 1                   |
Probability: | 100 %               |

Action:      | Apply 0.35 Stumble to All Enemies for 1 Turn. |
Cardinality: | 2                             |
Probability: | 50 %                          |

Action:      | Deal 3 Base Damage to All Enemies. |
Cardinality: | 2                   |
Probability: | 25 %                |

Action:      | Gain 1 Damage. |
Cardinality: | 2              |
Probability: | 25 %           |)";

  MovePool move_pool = {0};
  auto result = parse_enemy_move_pool(input_move_pool, &move_pool);
  printf("`%s`\n", result);
  ASSERT_EQ(result[0], '\0');

  free_move_pool(move_pool);
}

extern "C" char * cat_enemy_debuff_to_stream(char * to, Action a);

TEST(to_stream, enemy_debuff_to_stream_0) {
  auto input_card_pool = R"(Name:   | Bad Luck Kitty                                 |
Cost:   | 1                                              |
Rarity: | Common                                         |
Action: | Apply 0.65 Corrode to All Enemies for 2 Turns. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_enemy_debuff_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Apply 0.65 Corrode to All Enemies for 2 Turns.");

  free_card_pool(card_pool);
}

TEST(to_stream, enemy_debuff_to_stream_1) {
  auto input_card_pool = R"(Name:   | Bad Luck Kitty                                 |
Cost:   | 1                                              |
Rarity: | Common                                         |
Action: | Apply 0.651 Stumble to All Enemies for 1 Turn. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_enemy_debuff_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Apply 0.65 Stumble to All Enemies for 1 Turn.");

  free_card_pool(card_pool);
}

TEST(to_stream, enemy_debuff_to_stream_2) {
  auto input_card_pool = R"(Name:   | Bad Luck Kitty                                 |
Cost:   | 1                                              |
Rarity: | Common                                         |
Action: | Apply 6.5e-1 Corrode to All Enemies. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_enemy_debuff_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Apply 0.65 Corrode to All Enemies.");

  free_card_pool(card_pool);
}

extern "C" char * cat_self_buff_to_stream(char * to, Action a);

TEST(to_stream, self_buff_0) {
  auto input_card_pool = R"(Name:   | Zoomies        |
Cost:   | 1              |
Rarity: | UnCommon       |
Action: | Gain 1 Damage. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_self_buff_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Gain 1.00 Damage.");

  free_card_pool(card_pool);
}

TEST(to_stream, self_buff_1) {
  auto input_card_pool = R"(Name:   | Zoomies        |
Cost:   | 1              |
Rarity: | UnCommon       |
Action: | Gain 1.00000 Damage for 1 Turn. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_self_buff_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Gain 1.00 Damage for 1 Turn.");

  free_card_pool(card_pool);
}

TEST(to_stream, self_buff_2) {
  auto input_card_pool = R"(Name:   | Zoomies        |
Cost:   | 1              |
Rarity: | UnCommon       |
Action: | Gain 1e0 Damage for 2 Turns. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_self_buff_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Gain 1.00 Damage for 2 Turns.");

  free_card_pool(card_pool);
}

extern "C" char * cat_attack_to_stream(char * to, Action a);

TEST(to_stream, attack_0) {
  auto input_card_pool = R"(Name:   | Cataclysm                            |
Cost:   | 3                                    |
Rarity: | Summ                                 |
Action: | Deal 40 Base Damage to Target Enemy. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_attack_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Deal 40.00 Base Damage to Target Enemy.");

  free_card_pool(card_pool);
}

TEST(to_stream, attack_1) {
  auto input_card_pool = R"(Name:   | Cataclysm                            |
Cost:   | 3                                    |
Rarity: | Summ                                 |
Action: | Deal 4E+1 Base Damage to All Enemies. |)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  char tmp[MAX_CARD_TEXT] = {0};

  cat_attack_to_stream(tmp, card_pool.cards[0].actions[0]);

  ASSERT_STREQ(tmp, "Deal 40.00 Base Damage to All Enemies.");

  free_card_pool(card_pool);
}

TEST(to_stream, general_0) {
  auto input_card_pool = R"(Name:   | All Out Cattack                      |
Cost:   | 3                                    |
Rarity: | Summ                                 |
Action: |
Deal 4E+1 Base Damage to All Enemies.Gain 1e0 Damage for 2 Turns.
Apply 6.5e-1 Corrode to All Enemies.
|)";

  CardPool card_pool = {0};
  parse_self_card_pool(input_card_pool, &card_pool);

  auto card_streams = card_to_stream(card_pool.cards[0]);

  ASSERT_STREQ(card_streams.name, "All Out Cattack");
  ASSERT_STREQ(card_streams.cost, "3.00");
  ASSERT_STREQ(card_streams.rarity, "Summ");
  ASSERT_STREQ(card_streams.action_texts[0], "Deal 40.00 Base Damage to All Enemies.");
  ASSERT_STREQ(card_streams.action_texts[1], "Gain 1.00 Damage for 2 Turns.");
  ASSERT_STREQ(card_streams.action_texts[2], "Apply 0.65 Corrode to All Enemies.");

  free_card_pool(card_pool);
}
