#include "../header/interrupt.h"


void processorLocalTimerInterrupt (void)
{
    scheduler();
    //acknowledgement(risettaggio del timer) viene fatto all'interno dello scheduler
    
}
void intervalTimerInterrupt(void) {
    //semaforo di waitclock
    //svuoto la lista di processi
    //e li inserisco nella ready queue
    //incrementare il program counter di 4(stesso processo)
}

