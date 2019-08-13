#ifndef UTILS_H
#define UTILS_H
#include <umps/libumps.h>
#include <umps/types.h>
#include "./pcb.h"

#include "./const_rikaya.h"

//indirizzi new area
#define SYS_BP_NEW_AREA         0x200003D4
#define TRAP_NEW_AREA           0x200002BC
#define TLB_NEW_AREA            0x200001A4
#define INTERRUPT_NEW_AREA      0x2000008C
//indirizzi old area
#define SYS_BP_OLD_AREA         0x20000348
#define TRAP_OLD_AREA           0x20000230
#define TLB_OLD_AREA            0x20000118
#define INTERRUPT_OLD_AREA      0x20000000

#define FRAMESIZE 4096

//locazione di memoria del interval timer
#define INTERVAL_TIMER_MEM 0x10000020

//Interrupt line
#define PROC_LOCAL_TIMER_LINE 2
#define INTERVAL_TIMER_LINE 4

//Shif di CauseIp
#define LEFT_SHIFT_IP 16
#define RIGHT_SHIFT_IP 8
//Shift Cause.ExcCode
#define LEFT_SHIFT_EXCCODE 25
#define RIGHT_SHIFT_EXCCODES 2

#define EXCCODE_SYSCALL 8

//codici syscall

#define GETCPUTIME 1
#define CREATEPROCESS 2
#define TERMINATEPROCESS 3
#define VERHOGEN 4
#define PASSEREN 5
#define WAITCLOCK 6
#define IOCOMMAND 7
#define SETTUTOR 8
#define SPECPASSUP 9
#define GETPID 10

int maskBit(int variabile, int operazione, int bitPosition);
void copyState(state_t *origin, struct pcb_t *destination);
int getCauseField(int leftShift, int rightShift);
//timers
void setProcessorLocalTimer(int numMilliSeconds);
void setIntervalTimer(int numMilliseconds);
//ridare il controllo al processo senza chiamare lo scheduler
void incrementProgramCounter(void);

#endif //UTILS_H

