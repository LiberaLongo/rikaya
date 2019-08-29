#include "../header/interrupt.h"

void processorLocalTimerInterrupt(void)
{  
    //acknowledgement(risettaggio del timer) viene fatto all'interno dello scheduler
    scheduler();
}

void intervalTimerInterrupt()
{
    //rilascio tutti i processi bloccati per la WAITCLOCK
    while (headBlocked(&deviceSem[CLOCK_SEM]) != NULL)
    {
        verhogen(&deviceSem[CLOCK_SEM]);
    }
    //semaforo di WAITCLOCK
    setIntervalTimer(100);
    //svuoto la lista di processi
    //resettare interval timer
    //incrementare il program counter di 4(stesso processo)
    //LDST currentPcb->newrea
    //ripartire il processo corrente
}

