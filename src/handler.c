#include <umps/types.h>
#include "../header/utils.h"
#include "../header/interrupt.h"
#include "../header/syscall.h"

extern struct pcb_t * currentPcb;
/*
#define reg_a0 gpr[3]
#define reg_a1 gpr[4]
#define reg_a2 gpr[5]
#define reg_a3 gpr[6]
*/
void sys_bp_handler(void)
{
    //SYS/BP

    struct state_t *oldArea = (struct state_t *)SYS_BP_OLD_AREA;
    copyState(oldArea, currentPcb);
    
    if (getCauseField(LEFT_SHIFT_EXCCODE, RIGHT_SHIFT_EXCCODES) == EXCCODE_SYSCALL)
    {
        int a0 = currentPcb->p_s.gpr[3];
        switch (a0)
        {
        case TERMINATEPROCESS:
            terminateProcess();
            break;
        default:
            PANIC();
            break;
        }
    }else
    {
        PANIC();
    }
    //controllo causeExcCode
}

void trap_handler(void)
{
    //Trap
    PANIC();
}

void tlb_handler(void)
{
    //TLB
    PANIC();
}

void interrupt_handler(void)
{
    //copiare stato dalla old area al pcb del processo corrente
    struct state_t *oldArea = (struct state_t *)INTERRUPT_OLD_AREA;
    copyState(oldArea, currentPcb);
    //identificare la linea con Cause.Ip (controllo che sia il timer)
    int causeIP = getCauseField(LEFT_SHIFT_IP, RIGHT_SHIFT_IP);

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

    //NB: chiamare lo scheduler dalla funzione che gestisce l'interupt è una scelta progettuale
}

/*
mi scusi, ma quando viene chiamata un eccezione di tipo syscall, nell'old area realtiva vengono caricati
i relativi campi a0,a1 ecc ma effettivamente questi campi dove si trovano? nel gpr?
*/