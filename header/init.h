#ifndef INIT_H
#define INIT_H
#include <umps/types.h>
#include "./types_rikaya.h"
#include "./p15test.h"

struct list_head *ready_queue_h;
struct pcb_t * currentPcb;
//funzioni
void initNewArea(memaddr area, memaddr handler);
void setProcess(struct pcb_t *pcb, memaddr function, int priority);
void initialization(void);
#endif /*INIT_H*/