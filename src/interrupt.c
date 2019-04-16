#include <umps/types.h>
#include "../header/scheduler.h"


void timerInterruptManagement (void)
{
    //passare il controllo allo scheduler
    scheduler();
    //Acknowledgment da fare !!!!
    
}