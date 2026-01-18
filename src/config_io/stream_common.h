#ifndef STREAM_COMMON_H
#define STREAM_COMMON_H

#include <stddef.h>
#include <stdint.h>

#include "parsing_common_state.h"

#define MAX_ACTIONS   8
#define MAX_ACTION_TEXT 256

#define KW_SPACE " "

void cat_double_to_str(char * to, const double from);

// Public API
#ifdef __cplusplus
extern "C" {
#endif

void action_to_stream(char * to, Action a);
size_t max_action_text_len(char action_texts[][MAX_ACTION_TEXT],
    uint8_t qty_actions);

#ifdef __cplusplus
}
#endif

#endif
