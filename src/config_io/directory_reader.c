#include "directory_reader.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_curr_or_prev(const char * name) {
  return name[0] == '.'
    && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0'));
}

void cat_dir_connector(char * src) {
  size_t len = strnlen(src, MAX_DIR_LEN);
  if (src[len - 1] != DIR_CONNECTOR_CHAR) {
    strncat(src + 1, DIR_CONNECTOR, LEN_DIR_CONNECTOR);
  }
}

void cat_sub_dir(char * root_buf, const char * sub_dir) {
  size_t sub_dir_len = strnlen(sub_dir, MAX_DIR_LEN);
  strncat(root_buf, sub_dir, sub_dir_len);
  cat_dir_connector(root_buf);
}

void clear_sub_dir(char * root_buf, size_t root_buf_len) {
  size_t curr_root_buf_len = strnlen(root_buf, MAX_DIR_LEN);
  memset(root_buf + root_buf_len, 0, curr_root_buf_len - root_buf_len);
}

GameState read_configuration(const char * root) {
  GameState state = {0};
  DIR * dir = opendir(root);

  if (!dir) {
    fprintf(stderr, "read_configuration: could not open dir: `%s`\n", root);
    exit(1);
  }

  struct dirent * entry = {0};

  // This will be used as the storage for the root direcory and will be used to
  // get the dirs to set up the states below
  char root_buf[MAX_DIR_LEN] = {0};
  size_t root_len = strnlen(root, MAX_DIR_LEN);
  strncat(root_buf, root, root_len);
  cat_dir_connector(root_buf);
  size_t root_buf_len = strnlen(root_buf, MAX_DIR_LEN);

  while (entry = readdir(dir)) {

    if (is_curr_or_prev(entry->d_name)) {
      continue;
    }
    size_t sub_dir_len = strnlen(entry->d_name, MAX_DIR_LEN);

    if (entry->d_type == DT_DIR
        && !strncmp(entry->d_name, SELF_DIR_PREFIX, LEN_SELF_DIR_PREFIX)) {
      cat_sub_dir(root_buf, entry->d_name);
      state.self_state = init_self_state(root_buf);
      clear_sub_dir(root_buf, root_buf_len);
    } else if (entry->d_type == DT_DIR
        && !strncmp(entry->d_name, ENEMY_DIR_PREFIX, LEN_ENEMY_DIR_PREFIX)) {
      cat_sub_dir(root_buf, entry->d_name);
      add_enemy(&state, init_enemy_state(root_buf));
      clear_sub_dir(root_buf, root_buf_len);
    }
  }

  closedir(dir);
  return state;
}
