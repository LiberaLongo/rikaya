#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <umps/libumps.h>
#include "./listx.h"
#include <umps/types.h>
#include "./utils.h"
#include "./pcb.h"
#include "./init.h"

#include "./const_rikaya.h"

void aging(struct list_head *head);
void scheduler(void);

#endif /*SCHEDULER_H*/