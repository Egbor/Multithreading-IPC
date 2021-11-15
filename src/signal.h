#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <unistd.h>

void set_signal(int* signals, int count, void (*handler)(int));

#endif
