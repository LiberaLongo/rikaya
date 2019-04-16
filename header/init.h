#ifndef INIT_H
#define INIT_H
#include <umps/types.h>
#include "./types_rikaya.h"
void initNewArea(memaddr area, memaddr handler);
void setProcess(struct pcb_t *pcb, memaddr function, int priority);
void initialization(void);
#endif /*INIT_H*/