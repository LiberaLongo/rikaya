#ifndef SYSCALL_H
#define SYSCALL_H
#include <umps/types.h>
#include "./pcb.h"
#include "./scheduler.h"
#include "./types_rikaya.h"

void getCpuTime(unsigned int a1, unsigned int a2, unsigned int a3);
void createProcess(unsigned int a1, unsigned int a2, unsigned int a3);
void terminateProcess(unsigned int a1);
void verhogen(unsigned int a1);
void passeren(unsigned int a1);
void waitClock(void);
void IOCommand(unsigned int a1, unsigned int a2, unsigned int a3);
void setTutor(void);
void specPassUp(unsigned int a1, unsigned int a2, unsigned int a3);
void getPid(unsigned int a1, unsigned int a2);

#endif /*SYSCALL_H*/