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

//Interrupt line 1-7

//#define INTER_PROCESSOR 1       //LINEA
#define PROC_LOCAL_TIMER_LINE 2     //1
#define INTERVAL_TIMER_LINE 4       //2
#define DISK_DEVICE_LINE 8          //3
#define TAPE_DEVICE_LINE 16         //4
#define NETWORK_DEVICE_LINE 32      //5
#define PRINTER_DEVICE_LINE 64      //6
#define TERMINAL_DEVICES_LINE 128   //7

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

#define LINES 5 // linee: 3,4,5,6,7

#define DEV_LINE 8 //massimo numero di device per interrupt line

//massimo numero di devices: i terminali sono 2 device ciascuno  quindi
//riaggiungiamo DEV_LINE device più uno per il wait clock
#define MAX_DEVICES (LINES * DEV_LINE) + DEV_LINE + 1

// l'ultimo semaforo è per il wait clock
#define CLOCK_SEM (MAX_DEVICES - 1)

int maskBit(int variabile, int operazione, int bitPosition);
void copyState(state_t *origin, struct pcb_t *destination);
int getCauseField(int leftShift, int rightShift);
//timers
void setProcessorLocalTimer(int numMilliSeconds);
void setIntervalTimer(int numMilliseconds);
//ridare il controllo al processo senza chiamare lo scheduler
void incrementProgramCounter(void);

#endif //UTILS_H

