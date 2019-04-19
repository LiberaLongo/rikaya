#include "../header/interrupt.h"


void timerInterruptManagement (void)
{
    scheduler();
    //acknowledgement(risettaggio del timer) viene fatto all'interno dello scheduler
    
}