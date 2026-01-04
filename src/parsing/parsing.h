#ifndef PARSING_H
#define PARSING_H

#include <stdarg.h>
#include <stdint.h>

#define MAX_KW_LEN 64

const char * parse_ws(const char * input);
const char * parse_number(const char * input, double * d);
const char * parse_keyword(const char * input, const char * kw);
const char * parse_keywords(const char * input, uint8_t qty, ...);

#endif
