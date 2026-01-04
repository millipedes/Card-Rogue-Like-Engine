#include "parsing.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_whitespace(const char c) {
  return (c == ' ')
    || (c == '\t')
    || (c == '\n')
    || (c == '\r')
    || (c == '\f')
    || (c == '\v');
}

const char * parse_ws(const char * input) {
  if (input) {
    uint16_t inc = 0;
    while (is_whitespace(*(input + inc)) || *(input + inc) == '#') {
      if (*(input + inc) == '#') {
        while (*(input + inc) != '\n') {
          inc++;
        }
      }
      if (is_whitespace(*(input + inc))) {
        inc++;
      }
    }
    return input + inc;
  } else return NULL;
}

const char * parse_number(const char * input, double * d) {
  if (input) {
    uint16_t inc = 0;

    if (*(input + inc) == '-' || *(input + inc) == '+') inc++;

    // decimal
    while (isdigit(*(input + inc))) inc++;

    if (*(input + inc) == '.') inc++;

    // fractional
    while (isdigit(*(input + inc))) inc++;

    if ((*(input + inc) == 'e' || *(input + inc) == 'E')
        &&    (*(input + inc + 1) == '-'
            || *(input + inc + 1) == '+'
            || isdigit(*(input + inc + 1)))) {
      inc++;
      if (*(input + inc) == '-' || *(input + inc) == '+') inc++;
    }

    // exponent magnitude
    while (isdigit(*(input + inc))) inc++;

    if (inc > 0) {
      *d = strtod(input, NULL);
      return input + inc;
    } else return NULL;
  } else return NULL;
}

const char * parse_keyword(const char * input, const char * kw) {
  size_t len = strnlen(kw, MAX_KW_LEN);
  if (!strncmp(input, kw, len)) {
    return input + len;
  } else return NULL;
}

const char * parse_keywords(const char * input, uint8_t qty, ...) {
  va_list args;
  const char * result = NULL;

  va_start(args, qty);

  for (uint8_t i = 0; i < qty; i++) {
    if (!(result = parse_keyword(i == 0 ? parse_ws(input) : parse_ws(result),
            va_arg(args, const char *)))) {
      return NULL;
    }
  }

  va_end(args);

  return result;
}
