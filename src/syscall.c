#include "../header/syscall.h"

extern struct pcb_t * currentPcb;

//rimozione del processo corrente dalla readyQueue e caricamento del processo successivo
void terminateProcess(void)
{
    outProcQ(ready_queue_h, currentPcb);
    scheduler();
}