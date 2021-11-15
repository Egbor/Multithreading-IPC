#include "process.h"
#include "error.h"

#include <errno.h>
#include <stdlib.h>
//#include <memory.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int create_process(int(*process_entry)(void*), void* arg) {
  pid_t pid = fork();

  if (pid == -1) {
    throw_error("The error was caused while creating a process", -1);
  }
  
  if (pid == 0) {
    int exit_code = process_entry(arg);
    exit(exit_code);
  }
  
  return pid;
}

void wait_all_processes() {
  int status = 0;
  do {
    errno = 0;
    wait(&status);
  } while(errno != ECHILD);
}
