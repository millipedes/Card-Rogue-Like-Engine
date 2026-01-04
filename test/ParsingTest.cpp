#include <gtest/gtest.h>

#include <iostream>

#include "parsing/parsing.h"
#include "parsing/parsing_self_card.h"

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

extern "C" const char * parse_attack(const char * input, CardValue * value);

TEST(attack, attack_0) {
  CardValue value = {0};
  auto result = parse_attack("Deal 3.2 Base Damage to All Enemies.", &value);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.attack.magnitude, 3.2, eps);
  ASSERT_EQ(value.effect.type, ATTACK);
  ASSERT_EQ(value.target, ALL_ENEMIES);
}

TEST(attack, attack_1) {
  CardValue value = {0};
  auto result = parse_attack("Deal 3.2e-1 Base Damage to Target Enemy.", &value);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.attack.magnitude, 3.2e-1, eps);
  ASSERT_EQ(value.effect.type, ATTACK);
  ASSERT_EQ(value.target, TARGET_ENEMY);
}

extern "C" const char * parse_self_buff(const char * input, CardValue * value);

TEST(self_buff, self_buff_0) {
  CardValue value = {0};
  auto result = parse_self_buff("Gain 3.2e-1 Defense.", &value);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.self_buff.magnitude, 3.2e-1, eps);
  ASSERT_EQ(value.effect.value.self_buff.duration.type, INDEFINITE);
  ASSERT_EQ(value.effect.value.self_buff.duration.value.indefinite, true);
  ASSERT_EQ(value.effect.value.self_buff.type, DEFENSE);
  ASSERT_EQ(value.effect.type, SELF_BUFF);

  ASSERT_EQ(value.target, SELF);
}

TEST(self_buff, self_buff_1) {
  CardValue value = {0};
  auto result = parse_self_buff("Gain 3.2e-1 Defense for 1 Turn.", &value);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.self_buff.magnitude, 3.2e-1, eps);
  ASSERT_EQ(value.effect.value.self_buff.duration.type, FINITE);
  ASSERT_EQ(value.effect.value.self_buff.duration.value.remaining_turns, 1);
  ASSERT_EQ(value.effect.value.self_buff.type, DEFENSE);
  ASSERT_EQ(value.effect.type, SELF_BUFF);

  ASSERT_EQ(value.target, SELF);
}

TEST(self_buff, self_buff_2) {
  CardValue value = {0};
  auto result = parse_self_buff("Gain 3.2e-1 Defense for 3 Turns.", &value);
  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.self_buff.magnitude, 3.2e-1, eps);
  ASSERT_EQ(value.effect.value.self_buff.duration.type, FINITE);
  ASSERT_EQ(value.effect.value.self_buff.duration.value.remaining_turns, 3);
  ASSERT_EQ(value.effect.value.self_buff.type, DEFENSE);
  ASSERT_EQ(value.effect.type, SELF_BUFF);

  ASSERT_EQ(value.target, SELF);
}

extern "C" const char * parse_enemy_debuff(const char * input, CardValue * value);

TEST(enemy_debuff, enemy_debuff_0) {
  CardValue value = {0};
  auto result = parse_enemy_debuff("Apply 0.35 Stumble to All Enemies.", &value);

  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(value.effect.value.enemy_debuff.duration.type, INDEFINITE);
  ASSERT_EQ(value.effect.value.enemy_debuff.duration.value.indefinite, true);
  ASSERT_EQ(value.effect.value.enemy_debuff.type, STUMBLE);
  ASSERT_EQ(value.effect.type, ENEMY_DEBUFF);

  ASSERT_EQ(value.target, ALL_ENEMIES);
}

TEST(enemy_debuff, enemy_debuff_1) {
  CardValue value = {0};
  auto result = parse_enemy_debuff("Apply 0.35 Corrode to Target Enemy for 1 Turn.", &value);

  ASSERT_EQ(result[0], '\0');

  ASSERT_NEAR(value.effect.value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(value.effect.value.enemy_debuff.duration.type, FINITE);
  ASSERT_EQ(value.effect.value.enemy_debuff.duration.value.remaining_turns, 1);
  ASSERT_EQ(value.effect.value.enemy_debuff.type, CORRODE);
  ASSERT_EQ(value.effect.type, ENEMY_DEBUFF);

  ASSERT_EQ(value.target, TARGET_ENEMY);
}

TEST(card_action, card_action_0) {
  Action action = {0};
  const char * input = R"(
# This is a really broken card :)
Deal 9.1 Base Damage to Target Enemy.
Deal 6.7 Base Damage to All Enemies.
Gain 100 Damage. Gain 40 Defense for 1 Turn. Gain 95 Damage for 2 Turns.
Apply 0.35 Corrode to All Enemies.
Apply 0.95 Stumble to All Enemies for 100 Turns.
Apply 1.00 Corrode to Target Enemy for 1e1 Turns.
)";
  auto result = parse_card_actions(input, &action);
  ASSERT_EQ(result[0], '\0');

  ASSERT_EQ(action.qty_values, 8);

  // Attack Values
  ASSERT_NEAR(action.values[0].effect.value.attack.magnitude, 9.1, eps);
  ASSERT_EQ(action.values[0].effect.type, ATTACK);
  ASSERT_EQ(action.values[0].target, TARGET_ENEMY);
  ASSERT_NEAR(action.values[1].effect.value.attack.magnitude, 6.7, eps);
  ASSERT_EQ(action.values[1].effect.type, ATTACK);
  ASSERT_EQ(action.values[1].target, ALL_ENEMIES);

  // Self Buff Values
  ASSERT_NEAR(action.values[2].effect.value.self_buff.magnitude, 1e2, eps);
  ASSERT_EQ(action.values[2].effect.value.self_buff.duration.type, INDEFINITE);
  ASSERT_EQ(action.values[2].effect.value.self_buff.duration.value.indefinite, true);
  ASSERT_EQ(action.values[2].effect.value.self_buff.type, DAMAGE);
  ASSERT_EQ(action.values[2].effect.type, SELF_BUFF);
  ASSERT_EQ(action.values[2].target, SELF);

  ASSERT_NEAR(action.values[3].effect.value.self_buff.magnitude, 4e1, eps);
  ASSERT_EQ(action.values[3].effect.value.self_buff.duration.type, FINITE);
  ASSERT_EQ(action.values[3].effect.value.self_buff.duration.value.remaining_turns, 1);
  ASSERT_EQ(action.values[3].effect.value.self_buff.type, DEFENSE);
  ASSERT_EQ(action.values[3].effect.type, SELF_BUFF);
  ASSERT_EQ(action.values[3].target, SELF);

  ASSERT_NEAR(action.values[4].effect.value.self_buff.magnitude, 95, eps);
  ASSERT_EQ(action.values[4].effect.value.self_buff.duration.type, FINITE);
  ASSERT_EQ(action.values[4].effect.value.self_buff.duration.value.remaining_turns, 2);
  ASSERT_EQ(action.values[4].effect.value.self_buff.type, DAMAGE);
  ASSERT_EQ(action.values[4].effect.type, SELF_BUFF);
  ASSERT_EQ(action.values[4].target, SELF);

  // Enemy Debuff Values
  ASSERT_NEAR(action.values[5].effect.value.enemy_debuff.magnitude, 3.5e-1, eps);
  ASSERT_EQ(action.values[5].effect.value.enemy_debuff.duration.type, INDEFINITE);
  ASSERT_EQ(action.values[5].effect.value.enemy_debuff.duration.value.indefinite, true);
  ASSERT_EQ(action.values[5].effect.value.enemy_debuff.type, CORRODE);
  ASSERT_EQ(action.values[5].effect.type, ENEMY_DEBUFF);
  ASSERT_EQ(action.values[5].target, ALL_ENEMIES);

  ASSERT_NEAR(action.values[6].effect.value.enemy_debuff.magnitude, 9.5e-1, eps);
  ASSERT_EQ(action.values[6].effect.value.enemy_debuff.duration.type, FINITE);
  ASSERT_EQ(action.values[6].effect.value.enemy_debuff.duration.value.remaining_turns, 100);
  ASSERT_EQ(action.values[6].effect.value.enemy_debuff.type, STUMBLE);
  ASSERT_EQ(action.values[6].effect.type, ENEMY_DEBUFF);
  ASSERT_EQ(action.values[6].target, ALL_ENEMIES);

  ASSERT_NEAR(action.values[7].effect.value.enemy_debuff.magnitude, 1e0, eps);
  ASSERT_EQ(action.values[7].effect.value.enemy_debuff.duration.type, FINITE);
  ASSERT_EQ(action.values[7].effect.value.enemy_debuff.duration.value.remaining_turns, 10);
  ASSERT_EQ(action.values[7].effect.value.enemy_debuff.type, CORRODE);
  ASSERT_EQ(action.values[7].effect.type, ENEMY_DEBUFF);
  ASSERT_EQ(action.values[7].target, TARGET_ENEMY);

  free_action(action);
}
