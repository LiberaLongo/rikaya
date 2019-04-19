#ifndef UTILS_H
#define UTILS_H
#include <umps/libumps.h>
#include <umps/types.h>
#include "./pcb.h"

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

//Interrupt line
#define PROC_LOCAL_TIMER_LINE 2

//Shif di CauseIp
#define LEFT_SHIFT_IP 16
#define RIGHT_SHIFT_IP 8
//Shift Cause.ExcCode
#define LEFT_SHIFT_EXCCODE 25
#define RIGHT_SHIFT_EXCCODES 2

#define EXCCODE_SYSCALL 8

//codici syscall
#define TERMINATEPROCESS 3

int maskBit(int variabile, int operazione, int bitPosition);
void copyState(state_t *origin, struct pcb_t *destination);
int getCauseField(int leftShift, int rightShift);

#endif //UTILS_H

