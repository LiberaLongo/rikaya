#include "../header/interrupt.h"

void processorLocalTimerInterrupt(void)
{
    scheduler();
    //acknowledgement(risettaggio del timer) viene fatto all'interno dello scheduler
}
void intervalTimerInterrupt()
{
    while (headBlocked != NULL)
    {
        verhogen()
    }
    //semaforo di WAITCLOCK
    //svuoto la lista di processi
    //resettare interval timer
    //incrementare il program counter di 4(stesso processo)
    //LDST currentPcb->newrea
    //ripartire il processo corrente
}

