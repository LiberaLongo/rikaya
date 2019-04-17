
#include "../header/syscall.h"

extern struct pcb_t * currentPcb;
struct list_head * ready_queue_h; // cos'è sta roba

void terminateProcess(void)
{
    //per terminarlo bisogna rimuoverlo "definitivamente" dalla ready-queue
    outProcQ(ready_queue_h, currentPcb);
    scheduler();
}