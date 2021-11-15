#include "basename.h"
#include <string.h>

char basename[256] = "";

void set_basename(const char* filename) {
  char* name = strrchr(filename, '/');
  strcpy(basename, name ? name + 1 : filename);
}

char* get_basename() {
  return basename;
}
