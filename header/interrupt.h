#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "./scheduler.h"

void processorLocalTimerInterrupt(void);
void intervalTimerInterrupt(void);
#endif /*INTERRUPT_H*/