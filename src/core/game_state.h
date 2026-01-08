#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>

#include "enemy.h"
#include "self.h"

typedef struct {
  Self self;
  Enemy * enemies;
  uint8_t qty_enemies;
} GameState;

#endif
