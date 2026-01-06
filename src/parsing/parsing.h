#ifndef PARSING_H
#define PARSING_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_KW_LEN 64

bool is_whitespace(const char c);
const char * parse_ws(const char * input);
const char * parse_keyword(const char * input, const char * kw);
const char * parse_keywords(const char * input, uint8_t qty, ...);

// Public API
#ifdef __cplusplus
extern "C" {
#endif

const char * parse_number(const char * input, double * d);

#ifdef __cplusplus
}
#endif

#endif
