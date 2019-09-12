#ifndef UTILS_H
#define UTILS_H
#include <umps/libumps.h>
#include <umps/types.h>
#include "./pcb.h"

#include "./const_rikaya.h"



int maskBit(int variabile, int operazione, int bitPosition);
void copyState(state_t *origin, state_t *destination);
int getCauseField(int leftShift, int rightShift);
//timers
void setProcessorLocalTimer(int numMilliSeconds);
void setIntervalTimer(int numMilliseconds);
//ridare il controllo al processo senza chiamare lo scheduler
void incrementProgramCounter(void);

//matematica
int pot(int a, int b);

#endif //UTILS_H

