#include "error.h"
#include "basename.h"

#include <stdio.h>
#include <stdlib.h>

void throw_error(const char* message, int code) {
  fprintf(stderr, "%s: %s\n", get_basename(), message);
  exit(code);
}

void throw_warning(const char* message) {
  fprintf(stderr, "%s: %s\n", get_basename(), message);
}
