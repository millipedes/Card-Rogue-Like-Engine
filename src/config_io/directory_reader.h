#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include "core/game_state.h"

#define DIR_CONNECTOR      "/"
#define DIR_CONNECTOR_CHAR '/'

#define LEN_DIR_CONNECTOR    strnlen(DIR_CONNECTOR, sizeof(DIR_CONNECTOR))
#define LEN_SELF_DIR_PREFIX  strnlen(SELF_DIR_PREFIX, sizeof(SELF_DIR_PREFIX))
#define LEN_ENEMY_DIR_PREFIX strnlen(ENEMY_DIR_PREFIX, sizeof(ENEMY_DIR_PREFIX))

#define MAX_DIR_LEN 256

GameState read_configuration(const char * root);

#endif
