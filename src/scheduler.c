
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
        struct list_head *iter;
        list_for_each(iter, head)
        {
            struct pcb_t *temp = container_of(iter, pcb_t, p_next);
            temp->priority += 1;
        }
    }
}

void scheduler(void)
{
#ifdef DEBUG
    termprint("appena entrato in scheduler\n", 0);
#endif

    //rimozione del processo in testa alla ready queue
    currentPcb = removeProcQ(ready_queue_h);

#ifdef DEBUG
    termprint("in scheduler dopo removeprocQ1\n", 0);

#endif
    aging(ready_queue_h);

#ifdef DEBUG
    termprint("in scheduler dopo aging\n", 0);

#endif
    if (currentPcb != NULL)
    {
#ifdef DEBUG
    termprint("in scheduler if\n", 0);
#endif
        currentPcb->priority = currentPcb->original_priority;
    }
    else
    {
        //ready queue vuota
        HALT();
    }
#ifdef DEBUG
    termprint("in scheduler dopo ifthenelse\n", 0);
#endif
    //riaggiungere il currentProcess alla ready_queue
    insertProcQ(ready_queue_h, currentPcb);

    //contava il processo in esecuzione in p1.5 test
    //log_process_order(currentPcb->original_priority);
#ifdef DEBUG
    termprint("in scheduler dopo insert procQ\n", 0);
#endif
    //aggiorno tempo user
    startTimeUser = getTODLO();

    //impostazione del timer e caricamento del processo
    setProcessorLocalTimer(3);
    LDST(&currentPcb->p_s);
}
