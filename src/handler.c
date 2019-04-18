
#include "../header/handler.h"

extern pcb_t * currentPcb;
/*
#define reg_a0 gpr[3]
#define reg_a1 gpr[4]
#define reg_a2 gpr[5]
#define reg_a3 gpr[6]
*/
void sys_bp_handler(void)
{

   // termprint("dentro syshandler\n", 0);

    //SYS/BP
    state_t *oldArea = (state_t *)SYS_BP_OLD_AREA;
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
    //termprint("sono passati 3ms\n", 0);
    //copiare stato dalla old area al pcb del processo corrente
    state_t *oldArea = (state_t *)INTERRUPT_OLD_AREA;
    copyState(oldArea, currentPcb);
    //termprint("stato copiato\n",0);
    //identificare la linea con Cause.Ip (controllo che sia il timer)
    int causeIP = getCauseField(LEFT_SHIFT_IP, RIGHT_SHIFT_IP);
    //in fase di debug dobbiamo usare termprint
    //quindi lo mascheriamo qui
    causeIP = maskBit(causeIP, 0, 7);
    //stampa causeIP locale
    //termprint("causeIP settato\n",0);
    switch (causeIP)
    {
    case PROC_LOCAL_TIMER_LINE:
        //termprint("switch entrato\n", 0);
        //e se ci sono altri interrupt oltre al processor local timer non funziona
        timerInterruptManagement();
        //termprint("interrupt management uscito\n", 0);
        break;

    default:
        //termprint("ip non trovato\n", 0);
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