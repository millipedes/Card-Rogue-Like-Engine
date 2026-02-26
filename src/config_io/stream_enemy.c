#include "stream_enemy.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

MoveStream move_to_stream(Move move) {
  MoveStream move_stream = {0};

  for (uint8_t i = 0; i < move.qty_actions; i++) {
    if (i > MAX_ACTIONS) {
      fprintf(stderr, "move_to_stream: max number of actions reached, will no "
          "longer be recording actions in output stream");
      break;
    }
    action_to_stream(move_stream.action_texts[i], move.actions[i]);
    move_stream.qty_actions++;
  }

  return move_stream;
}

ArtStreams enemy_state_to_art_streams(const EnemyState enemy) {
  ArtStreams art_streams = {0};

  char health_buf[ART_MAX_WIDTH] = {0};
  snprintf(health_buf, ART_MAX_WIDTH, "%.2f", enemy.health);

  art_streams.max_art_width = MAX(
      max_art_width(enemy.art_lines, enemy.qty_art_lines),
      MAX(
        strnlen(health_buf, ART_MAX_WIDTH),
        strnlen(enemy.name, ART_MAX_WIDTH)
      ));
  bool art_is_max_len = art_streams.max_art_width
    == max_art_width(enemy.art_lines, enemy.qty_art_lines);

  for (uint8_t i = 0; i < enemy.qty_art_lines; i++) {
    // If the art is the largest thing it should not be centered
    if (art_is_max_len) {
      art_streams.art[i][0] = CHAR_SPACE;
      strncpy(art_streams.art[i] + 1, enemy.art_lines[i],
          strnlen(enemy.art_lines[i], ART_MAX_WIDTH));
    } else {
      center_about_length(art_streams.art[i], enemy.art_lines[i], art_streams.max_art_width);
    }
  }
  center_about_length(art_streams.name, enemy.name, art_streams.max_art_width);
  center_about_length(art_streams.health, health_buf, art_streams.max_art_width);
  return art_streams;
}
