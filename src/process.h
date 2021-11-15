#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <sys/wait.h>

extern void wait_all_processes();
extern int create_process(int(*process_entry)(void*), void* arg);

#endif
