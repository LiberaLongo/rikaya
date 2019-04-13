#include <umps/types.h>
#include "../header/utils.h"

void sys_bp_handler()
{
    //SYS/BP
}

void trap_handler()
{
    //Trap
}

void tlb_handler()
{
    //TLB
}

void interrupt_handler()
{
    //copiare stato dalla old area al pcb del processo corrente
    struct state_t *oldArea = (struct state_t *)INTERRUPT_OLD_AREA;
    copyState(oldArea, currentPcb);
    //identificare la linea con Cause.Ip (controllo che sia il timer)
    int causeIP = getCauseIP();
    
    switch (causeIP)
    {
        case PROC_LOCAL_TIMER_LINE:
            //e se ci sono altri interrupt oltre al processor local timer non funziona
            timerInterruptManagement();
            break;
    
        default:
            PANIC();
            break;
    }
    //per la fase 2 bisognerà identificare il device se linea > 3
    //acknowledgement del interrupt
    //passare il controllo allo scheduler
}