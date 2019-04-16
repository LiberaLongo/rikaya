#include <umps/types.h>
#include "../header/pcb.h"
#include "../header/scheduler.h"

extern struct pcb_t * currentPcb;


void terminateProcess(void)
{
    //per terminarlo bisogna rimuoverlo "definitivamente" dalla ready-queue
    outProcQ(ready_queue_h, currentPcb);
    scheduler();
}