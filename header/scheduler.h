#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <umps/libumps.h>

#include "./p15test.h"

void timerSetting(int numMilliSeconds);
void aging(struct list_head *head);
void scheduler(void);

#endif /*SCHEDULER_H*/