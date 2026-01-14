#ifndef CORE_UTILITIES_H
#define CORE_UTILITIES_H

#include <stdint.h>

#define MAX_PATH_LEN 256
#define MAX_FILE_LEN (1024 * 1024)

#define ART_MAX_WIDTH 128

#define NEWLINE_TOK "\n"

char * make_file_name(const char * root_dir, const char * file_name);
char * dump_file_contents(const char * file_path);
void get_art_lines(char *** art_lines, uint8_t * qty_art_lines,
    const char * file_path);

#endif
