#include <umps/types.h>
#include "../header/utils.h"
#include "../header/listx.h"
#include "../header/pcb.h"
#include "../header/init.h"

struct pcb_t * currentPcb = NULL;
//codice scheduler

void timerSetting(int numMilliSeconds)
{
    //definito in p1.5test_rikaya_v0.c (riga 22)
    //#define TIME_SCALE *((unsigned int *)0x10000024)

    // da verificare cosa si ottiene con la moltiplicazione -> unsigned int
    setTIMER(numMilliSeconds * 1000 * TIME_SCALE);
}

void aging(struct list_head *head)
{
    if(head != NULL)
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
    //aggiornare il currentPcb
    //rimuovere il processo dalla ready_queue
    currentPcb = removeProcQ(ready_queue_h);
    
    //fare aging alla ready_queue
    aging(ready_queue_h);
    if(currentPcb != NULL) {
        //aggiornare currentPcb priority alla original_priority
        currentPcb->priority = currentPcb->original_priority;
    } else {
        //idle quindi chiamo ALT();
        HALT();
    }
    //riaggiungere il currentProcess alla ready_queue   
    insertProcQ(ready_queue_h, currentPcb);
    //NB: reinserisco il pcb in coda con priorità
    //perchè non voglio rubare tempo al processo e perchè ho già estratto il currentPcb di cui farò LDST

    //log_process_order
    log_process_order(currentPcb->original_priority);

    //settare il timer
    timerSetting(3);
    //LDST del currentPcb
    LDST(&currentPcb->p_s);
}
/*
simulazione:

ready queue p3 = 3, p2 = 2, p1 = 1

1

currentpcb = p3
aging p1 = 2, p2 = 3
ready queue p2 = 3, p3 = 3, p1 = 2

2

currentpcb = p2
aging p1 = 3, p3 = 4
ready queue p3 = 4, p1 = 3, p2 = 2

3

currentpcb = p3 
aging p1 = 4, p2 = 3
ready queue p1 = 4, p2 = 3, p3 = 3

4

currentpcb = p1
aging p2 = 4 , p3 = 4
ready queue p2 = 4, p3 = 4, p1 = 1

5

currentpcb = p2
aging p1 = 2, p3 = 5
ready queue p3 = 5, p1 = 2, p2 = 2

6

currentpcb = p3
aging p1 = 3, p2 = 3
ready queue p1 = 3, p2 = 3, p3 = 3

7

currentpcb = p1
aging p2 = 4, p3 = 4
ready queue p2 = 4, p3 = 4, p1 = 1

ritorno a 5

*/
