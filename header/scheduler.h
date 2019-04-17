#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <umps/libumps.h>
#include "./listx.h"
#include <umps/types.h>
#include "./utils.h"
#include "./pcb.h"
#include "./init.h"
#include "./p15test.h"

void timerSetting(int numMilliSeconds);
void aging(struct list_head *head);
void scheduler(void);

#endif /*SCHEDULER_H*/