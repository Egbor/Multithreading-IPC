#include "signal.h"

#include <signal.h>
#include <memory.h>

void set_signal(int* signals, int count, void (*handler)()) {
  struct sigaction action;
  sigset_t set;
  
  memset(&action, 0, sizeof(struct sigaction));

  action.sa_handler = handler;
  action.sa_mask = set;

  sigemptyset(&set);
  for (int i = 0; i < count; i++) {
    sigaddset(&set, signals[i]);
    sigaction(signals[i], &action, 0);
  }

}
