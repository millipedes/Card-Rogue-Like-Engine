#include <gtest/gtest.h>

#include <string>

#include "core/self_state.h"

constexpr auto card_pool_input = R"(Name:   | Scratch                             |
Cost:   | 1                                   |
Rarity: | Common                              |
Action: | Deal 3 Base Damage to Target Enemy. |

Name:   | Hidey Hole                 |
Cost:   | 1                          |
Rarity: | Common                     |
Action: | Gain 2 Defense for 1 Turn. |)";
constexpr auto starting_deck_input = R"(Scratch: 7
Hidey Hole: 7)";

constexpr auto scratch_str = "Scratch";
constexpr auto hidey_hole_str = "Hidey Hole";

auto init_self_state_from_literal(const char * card_pool_input,
    const char * starting_deck_input) -> SelfState {
  SelfState self_state = {0};
  parse_self_card_pool(card_pool_input, &self_state.card_pool);
  parse_self_starting_deck(starting_deck_input, &self_state.card_pool,
      &self_state.deck);
  return self_state;
}

// With seed set to 42 which results in the following order for 1 shuffle:
//  0. Scratch
//  1. Hidey Hole
//  2. Scratch
//  3. Hidey Hole
//  4. Scratch
//  5. Hidey Hole
//  6. Scratch
//  7. Hidey Hole
//  8. Scratch
//  9. Scratch
// 10. Hidey Hole
// 11. Scratch
// 12. Hidey Hole
// 13. Hidey Hole

TEST(deck_shuffle, deck_shuffle_0) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, 0);

  ASSERT_STREQ(self_state.in_deck[0]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[1]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[2]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[3]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[4]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[5]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[6]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[7]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[8]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[9]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[10]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[11]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[12]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[13]->name, "Hidey Hole");
  ASSERT_EQ(self_state.qty_in_deck, 14);
  ASSERT_EQ(self_state.qty_hand, 0);
  ASSERT_EQ(self_state.qty_discard, 0);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_1) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);

  ASSERT_STREQ(self_state.in_deck[0]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[1]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[2]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[3]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[4]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[5]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[6]->name, "Scratch");
  ASSERT_STREQ(self_state.hand[6]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.hand[5]->name, "Scratch");
  ASSERT_STREQ(self_state.hand[4]->name, "Scratch");
  ASSERT_STREQ(self_state.hand[3]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.hand[2]->name, "Scratch");
  ASSERT_STREQ(self_state.hand[1]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.hand[0]->name, "Hidey Hole");
  ASSERT_EQ(self_state.qty_in_deck, 7);
  ASSERT_EQ(self_state.qty_hand, 7);
  ASSERT_EQ(self_state.qty_discard, 0);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_2) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);
  self_state = discard_hand(self_state);

  ASSERT_STREQ(self_state.in_deck[0]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[1]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[2]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[3]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[4]->name, "Scratch");
  ASSERT_STREQ(self_state.in_deck[5]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.in_deck[6]->name, "Scratch");
  ASSERT_STREQ(self_state.discard[6]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.discard[5]->name, "Scratch");
  ASSERT_STREQ(self_state.discard[4]->name, "Scratch");
  ASSERT_STREQ(self_state.discard[3]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.discard[2]->name, "Scratch");
  ASSERT_STREQ(self_state.discard[1]->name, "Hidey Hole");
  ASSERT_STREQ(self_state.discard[0]->name, "Hidey Hole");
  ASSERT_EQ(self_state.qty_in_deck, 7);
  ASSERT_EQ(self_state.qty_hand, 0);
  ASSERT_EQ(self_state.qty_discard, 7);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_3) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);
  self_state = discard_hand(self_state);
  self_state = draw_cards(self_state, 8);

  ASSERT_EQ(self_state.qty_in_deck, 6);
  ASSERT_EQ(self_state.qty_hand, 8);
  ASSERT_EQ(self_state.qty_discard, 0);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_4) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);
  self_state = discard_hand(self_state);
  self_state = draw_cards(self_state, 8);
  self_state = draw_cards(self_state, 6);

  ASSERT_EQ(self_state.qty_in_deck, 0);
  ASSERT_EQ(self_state.qty_hand, 14);
  ASSERT_EQ(self_state.qty_discard, 0);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_5) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);
  self_state = discard_hand(self_state);
  self_state = draw_cards(self_state, 8);
  self_state = draw_cards(self_state, 7);

  ASSERT_EQ(self_state.qty_in_deck, 0);
  ASSERT_EQ(self_state.qty_hand, 14);
  ASSERT_EQ(self_state.qty_discard, 0);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_6) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);
  self_state = discard_hand(self_state);
  self_state = draw_cards(self_state, 8);
  self_state = draw_cards(self_state, 7);
  self_state = draw_cards(self_state, 1);

  ASSERT_EQ(self_state.qty_in_deck, 0);
  ASSERT_EQ(self_state.qty_hand, 14);
  ASSERT_EQ(self_state.qty_discard, 0);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}

TEST(deck_shuffle, deck_shuffle_7) {
  srand(42);
  auto self_state = init_self_state_from_literal(card_pool_input,
      starting_deck_input);

  self_state = start_battle(self_state, STARTING_HAND_SIZE);
  self_state = discard_hand(self_state);
  self_state = draw_cards(self_state, 7);
  self_state = draw_cards(self_state, 1);
  self_state = discard_hand(self_state);
  self_state = draw_cards(self_state, 5);

  ASSERT_EQ(self_state.qty_in_deck, 1);
  ASSERT_EQ(self_state.qty_hand, 5);
  ASSERT_EQ(self_state.qty_discard, 8);
  ASSERT_EQ(self_state.qty_removed, 0);

  free_self_state(self_state);
}
