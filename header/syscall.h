#ifndef SYSCALL_H
#define SYSCALL_H
#include <umps/types.h>
#include "./pcb.h"
#include "./scheduler.h"
#include "./types_rikaya.h"

void getCpuTime(int a1, int a2, int a3)
void createProcess(int a1, int a2, int a3)
void terminateProcess(int a1);
void verhogen(int a1)
void passeren(int a1)
void waitClock(void)
void IOCommand(int a1, int a2, int a3)
void setTutor(void)
void specPassUp(int a1, int a2, int a3)
void getPid(int a1, int a2)

#endif /*SYSCALL_H*/