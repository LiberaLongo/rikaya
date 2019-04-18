#include "../header/interrupt.h"


void timerInterruptManagement (void)
{
    //termprint("chiamata scheduler dal interrupt\n", 0);
    //passare il controllo allo scheduler
    scheduler();
    //Acknowledgment da fare !!!!
    
}