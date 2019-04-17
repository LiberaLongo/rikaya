#include "../header/interrupt.h"


void timerInterruptManagement (void)
{
    //passare il controllo allo scheduler
    scheduler();
    //Acknowledgment da fare !!!!
    
}