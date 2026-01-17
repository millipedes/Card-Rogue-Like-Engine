#include "core_utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
