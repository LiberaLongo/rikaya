
#include "../header/scheduler.h"

struct pcb_t *currentPcb = NULL;

//variabili per il debug con traced region
//int prioritaCorrente , prioritaOriginale ;
//unsigned int indirizzoCorrente;
unsigned int startTimeUser = 0;
unsigned int startTimeKernel = 0;

//incrementate le priorita di tutti gli elementi presenti nella readyQueue
void aging(struct list_head *head)
{
    if (head != NULL)
    {
#ifdef DEBUG
        termprint("dentro aging prima\n", 0);
#endif
        struct list_head *iter;
        if (!list_empty(head))
        {
            list_for_each(iter, head)
            {
                struct pcb_t *temp = container_of(iter, pcb_t, p_next);
                temp->priority += 1;
            }
        }

#ifdef DEBUG
        termprint("dentro aging dopo\n", 0);
#endif
    }
}

void scheduler(void)
{
#ifdef DEBUG
    termprint("Scheduler\n", 0);
#endif

    //rimozione del processo in testa alla ready queue
    currentPcb = removeProcQ(ready_queue_h);
    aging(ready_queue_h);
    if (currentPcb != NULL)
    {
        currentPcb->priority = currentPcb->original_priority;
    }
    else
    {
#ifdef DEBUG
        termprint("HALT o IDLE!!!\n", 0);
#endif
        //ready queue vuota
        //HALT();

        while (1);
    }
    //riaggiungere il currentProcess alla ready_queue
    insertProcQ(ready_queue_h, currentPcb);

    //contava il processo in esecuzione in p1.5 test
    //log_process_order(currentPcb->original_priority);
    //aggiorno tempo user
    startTimeUser = getTODLO();

    //impostazione del timer e caricamento del processo
    setProcessorLocalTimer(3);
    LDST(&currentPcb->p_s);
}
