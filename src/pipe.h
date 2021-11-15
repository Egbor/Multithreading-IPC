#ifndef _PIPE_H_
#define _PIPE_H_

#include <stddef.h>

typedef struct _pipe pipe_t;

#define PIPE_READ_MODE 0
#define PIPE_WRITE_MODE 1

extern pipe_t* create_pipe();

extern void open_pipe(pipe_t* pipe, int mode);
extern void close_pipe(pipe_t* pipe);

extern int write_in_pipe(pipe_t* pipe, const void* buffer, size_t size);
extern int read_from_pipe(pipe_t* pipe, void* buffer, size_t size);

#endif
