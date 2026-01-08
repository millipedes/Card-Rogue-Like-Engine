#ifndef SELF_H
#define SELF_H

#include "parsing/parsing_self_card.h"

#define MAX_PATH_LEN 256
#define MAX_FILE_LEN (1024 * 1024)

#define SELF_ART_FILE_NAME           "art.txt"
#define SELF_CARD_POOL_FILE_NAME     "card_pool.txt"
#define SELF_STARTING_DECK_FILE_NAME "starting_deck.txt"

typedef struct {
  char * art;
  Deck deck;
  CardPool card_pool;
} Self;

Self init_self(const char * root_dir);
void shuffle_deck(Deck deck);
void free_self(Self self);

#endif
