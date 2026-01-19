#ifndef STREAM_COMMON_H
#define STREAM_COMMON_H

#include <stddef.h>
#include <stdint.h>

#include "parsing_enemy_state.h"
#include "parsing_self_state.h"

#define MAX_PATH_LEN        128
#define MAX_TEXT_STREAM_LEN 512

#define KW_SPACE " "
// Note because TextStream is internal we don't need to worry about the other
// nonsense new-line stuff like form feed or carriage return
#define KW_NL    "\n"

typedef char TextStream[MAX_TEXT_STREAM_LEN];

void cat_double_to_tstream(char * to, const double from);

// Public API
#ifdef __cplusplus
extern "C" {
#endif

void action_to_stream(char * to, Action a);
size_t max_tstream_len(TextStream tstream);
size_t max_tstream_height(TextStream tstream);

char * make_file_name(const char * root_dir, const char * file_name);
void dump_file_contents(TextStream tstream, const char * file_path);

void card_to_stream(TextStream tstream, Card card);
void hand_to_stream(TextStream tstream, CardPool card_pool);

void move_to_stream(TextStream tstream, Move move);

#ifdef __cplusplus
}
#endif

#endif
