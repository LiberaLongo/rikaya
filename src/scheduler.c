
#include "../header/scheduler.h"

struct pcb_t *currentPcb = NULL;

//variabili per il debug con traced region
//int prioritaCorrente , prioritaOriginale ;
//unsigned int indirizzoCorrente;

void timerSetting(int numMilliSeconds)
{
    //settaggio del timer in ms
    setTIMER(numMilliSeconds * 1000 * TIME_SCALE);
}

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
    //rimozione del processo in testa alla ready queue
    currentPcb = removeProcQ(ready_queue_h);
    aging(ready_queue_h);

    if (currentPcb != NULL)
    {
        currentPcb->priority = currentPcb->original_priority;
    }
    else
    {
        //ready queue vuota
        HALT();
    }
    //riaggiungere il currentProcess alla ready_queue
    insertProcQ(ready_queue_h, currentPcb);

    log_process_order(currentPcb->original_priority);
    //impostazione del timer e caricamento del processo
    timerSetting(3);
    LDST(&currentPcb->p_s);
}
