#include "pipe.h"
#include "error.h"

#include <stdlib.h>
#include <unistd.h>

typedef struct _pipe {
  int fd[2];
  int mode;
} pipe_t;

pipe_t* create_pipe() {
  pipe_t* p = (pipe_t*)malloc(sizeof(pipe_t));
  if (pipe(p->fd) == -1) {
    throw_error("The error was caused while creating a pipe", -1);
  }
  return p;
}

void open_pipe(pipe_t* pipe, int mode) {
  close(pipe->fd[!mode]); // reverse 'mode' to close the unused pipe end
  pipe->mode = !!mode; // do a double 'logical not' to normalize 'mode' in range from 0 to 1
}

void close_pipe(pipe_t* pipe) {
  close(pipe->fd[pipe->mode]);
}

int write_in_pipe(pipe_t* pipe, const void* buffer, size_t size) {
  if (pipe->mode == PIPE_READ_MODE) {
    throw_error("Unable to write in pipe such as it has been open in read mode", -1);
  }
  return write(pipe->fd[pipe->mode], buffer, size);
}

int read_from_pipe(pipe_t* pipe, void* buffer, size_t size) {
  if (pipe->mode == PIPE_WRITE_MODE) {
    throw_error("Unable to write in pipe such as it has been open in write mode", -1);
  }
  return read(pipe->fd[pipe->mode], buffer, size);
}
