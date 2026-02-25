#include "core_utilities.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char * make_entity_name(const char * root_dir, const char * entity_prefix) {
  char entity_name_buf[ART_MAX_WIDTH] = {0};
  char * entity_name_root = strstr(root_dir, entity_prefix);
  entity_name_root += strnlen(entity_prefix, ART_MAX_WIDTH);
  uint16_t root_inc = 0;
  uint16_t buf_inc = 0;
  while (isalpha(entity_name_root[root_inc])) {
    if (root_inc != 0 && isupper(entity_name_root[root_inc])) {
      entity_name_buf[buf_inc] = CHAR_SPACE;
      buf_inc++;
    }
    entity_name_buf[buf_inc] = entity_name_root[root_inc];
    buf_inc++;
    root_inc++;
  }
  size_t entity_name_buf_len = strnlen(entity_name_buf, ART_MAX_WIDTH);
  char * entity_name = calloc(entity_name_buf_len + 1, sizeof(char));
  strncpy(entity_name, entity_name_buf, entity_name_buf_len);
  return entity_name;
}

char * make_file_name(const char * root_dir, const char * file_name) {
  size_t root_dir_len = strnlen(root_dir, MAX_PATH_LEN);
  size_t file_name_len = strnlen(file_name, MAX_PATH_LEN);
  char * result = calloc(root_dir_len + file_name_len + 1, sizeof(char));
  strncpy(result, root_dir, root_dir_len);
  strncpy(result + root_dir_len, file_name, file_name_len);
  return result;
}

char * dump_file_contents(const char * file_path) {
  FILE * fp = fopen(file_path, "r");
  struct stat st;
  fstat(fileno(fp), &st);

  char * into = calloc(st.st_size + 1, sizeof(char));

  fread(into, 1, st.st_size, fp);
  fclose(fp);
  return into;
}

void add_art_line(char *** art_lines, uint8_t * qty_art_lines,
    const char * new_line) {
  if (!art_lines[0]) {
    *qty_art_lines = 1;
    art_lines[0] = calloc(1, sizeof(char *));
  } else {
    (*qty_art_lines)++;
    art_lines[0] = realloc(art_lines[0], *qty_art_lines * sizeof(char *));
  }
  size_t len = strnlen(new_line, ART_MAX_WIDTH);
  art_lines[0][*qty_art_lines - 1] = calloc(len + 1, sizeof(char));
  strncpy(art_lines[0][*qty_art_lines - 1], new_line, len);
}

void get_art_lines(char *** art_lines, uint8_t * qty_art_lines,
    const char * file_path) {
  FILE * fp = fopen(file_path, "r");
  struct stat st;
  fstat(fileno(fp), &st);

  char * into = calloc(st.st_size + 1, sizeof(char));

  fread(into, 1, st.st_size, fp);
  fclose(fp);

  char * tmp = NULL;
  do {
    if (!tmp) {
      tmp = strtok(into, NEWLINE_TOK);
    } else {
      tmp = strtok(NULL, NEWLINE_TOK);
    }
    if (tmp) {
      add_art_line(art_lines, qty_art_lines, tmp);
    }
  } while (tmp);
  free(into);
}

size_t max_art_width(char ** art_lines, uint8_t qty_art_lines) {
  size_t max_len = 0;
  for (uint8_t i = 0; i < qty_art_lines; i++) {
    size_t curr_len = strnlen(art_lines[i], ART_MAX_WIDTH);
    if (curr_len > max_len) {
      max_len = curr_len;
    }
  }
  return max_len;
}
