#include <stdio.h>
#include <stdlib.h>

#include "core/self.h"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./card_rl source_file_roo_dir/");
    exit(1);
  }

  Self self = init_self(argv[1]);
  free_self(self);
  return 0;
}
