#ifndef CORE_GRAPHICS_H
#define CORE_GRAPHICS_H

#include <ncurses.h>

#include "config_io/text_stream.h"

void init_ncurses();
WINDOW * autosize_window(WINDOW * parent, TextStream tstream, uint8_t per_y,
    uint8_t per_x);
void end_ncurses();

#endif
