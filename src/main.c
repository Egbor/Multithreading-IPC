#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/mman.h>

#include "basename.h"
#include "process.h"
#include "signal.h"
#include "error.h"
#include "pipe.h"

struct process_ids {
  int pid_a;
  int pid_b;
  int pid_c;
};

struct memory_desc {
  int value;
  bool sync;
};

struct process_arguments {
  pipe_t* pipe;
  struct memory_desc* shared_memory;
};

struct process_ids* ids;

bool terminate_status = false;
int signals[] = { SIGTERM, SIGUSR1 };

void term_signal_handler(int sig) {
  if (sig == SIGTERM) {
    terminate_status = true;
  }
}

void b_signal_handler(int sig) {
  if (sig == SIGUSR1) {
    kill(ids->pid_a, SIGTERM);
    kill(ids->pid_c, SIGTERM);
    kill(ids->pid_b, SIGTERM);
  } else {
    term_signal_handler(sig);
  }
}

void* c1_thread_entry(void* arg) {
  struct process_arguments* arguments = arg;

  while (!terminate_status) {
    if (arguments->shared_memory->sync) {
      printf("C value=%d\n", arguments->shared_memory->value);
      if (arguments->shared_memory->value == 100) {
	kill(ids->pid_b, SIGUSR1);
      }
      arguments->shared_memory->sync = false;
    }
  }
}

void* c2_thread_entry(void* arg) {
  struct process_arguments* arguments = arg;

  while (!terminate_status) {
    if (!arguments->shared_memory->sync) {
      printf("I am alive\n");
      sleep(1);
    }
  }
}

int a_process_entry(void* arg) {
  struct process_arguments* arguments = arg;
  int value = 0;

  set_signal(signals, 1, term_signal_handler);
  
  open_pipe(arguments->pipe, PIPE_WRITE_MODE);
  while (!terminate_status) {
    scanf("%d", &value);
    if (write_in_pipe(arguments->pipe, &value, sizeof(int)) == -1) {
      throw_warning("A");
    }
  }
  close_pipe(arguments->pipe);
  
  return 0;
}

int b_process_entry(void* arg) {
  struct process_arguments* arguments = arg;
  int value = 0;

  set_signal(signals, 2, b_signal_handler);
  
  open_pipe(arguments->pipe, PIPE_READ_MODE);
  while (!terminate_status) {
    if (read_from_pipe(arguments->pipe, &value, sizeof(int)) != -1) {
      arguments->shared_memory->value = value * value;
      arguments->shared_memory->sync = true;
    }
  }
  close_pipe(arguments->pipe);
  
  return 0;
}

int c_process_entry(void* arg) {
  pthread_t c1;
  pthread_t c2;

  set_signal(signals, 1, term_signal_handler);
  
  pthread_create(&c1, NULL, c1_thread_entry, arg);
  pthread_create(&c2, NULL, c2_thread_entry, arg);

  pthread_join(c1, NULL);
  pthread_join(c2, NULL);
  
  return 0;
}

int main(int argc, char* argv[]) {
  set_basename(argv[0]);

  pipe_t* pipe = create_pipe();
  struct memory_desc* shared_memory = mmap(NULL, sizeof(struct memory_desc),  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  struct process_ids* process_ids = mmap(NULL, sizeof(struct process_ids),  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  shared_memory->value = 0;
  shared_memory->sync = false;
  
  struct process_arguments arguments;
  arguments.pipe = pipe;
  arguments.shared_memory = shared_memory;

  ids = process_ids;

  ids->pid_a = create_process(a_process_entry, &arguments);
  ids->pid_b = create_process(b_process_entry, &arguments);
  ids->pid_c = create_process(c_process_entry, &arguments);
  
  wait_all_processes();
  
  free(pipe);
  munmap(shared_memory, sizeof(struct memory_desc));
  munmap(process_ids, sizeof(struct process_ids));
  
  return 0;
}
