#ifndef SYSCALL_H
#define SYSCALL_H
#include <umps/types.h>
#include "./pcb.h"
#include "./scheduler.h"

#define TERMINATEPROCESS 3
void terminateProcess(void);
#endif /*SYSCALL_H*/