#ifndef HANDLER_H
#define HANDLER_H
#include <umps/types.h>
#include "./utils.h"
#include "./interrupt.h"
#include "./syscall.h"
#include "./types_rikaya.h"

#include "./const_rikaya.h"

void sys_bp_handler(void);
void trap_handler(void);
void tlb_handler(void);
void interrupt_handler(void);
#endif /*HANDLER_H*/