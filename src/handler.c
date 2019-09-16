#include "../header/handler.h"

extern pcb_t *currentPcb;
/*Registri a0-a3
#define reg_a0 gpr[3]
#define reg_a1 gpr[4]
#define reg_a2 gpr[5]
#define reg_a3 gpr[6]
*/

//bisogna inizializzare startTimeKernel
extern unsigned int startTimeKernel;
extern unsigned int startTimeUser;

void specPassUpHandler(int type)
{
#ifdef DEBUG
    termprint("dentro a specpassupHandler\n", 0);
#endif
    if (currentPcb->newAreaHandler[type] != NULL)
    {
        state_t *oldArea;
        switch (type)
        {
        case SYSBK:
            //lo stato che gli passiamo era quello del current pcb che è stato salvato nell'old area delle syscall
            oldArea = (state_t *)SYSBK_OLDAREA;
            break;
        case TLB:
            oldArea = (state_t *)TLB_OLDAREA;
            break;
        case TRAP:
            oldArea = (state_t *)PGMTRAP_OLDAREA;
            break;
        default:
            HALT();
            break;
        }
        copyState(oldArea, currentPcb->oldAreaHandler[type]);
        //loadstate per eseguire l'handler
        LDST(currentPcb->newAreaHandler[type]);
    }
    else
        HALT();
}

void sys_bp_handler(void)
{
#ifdef DEBUG
    termprint("dentro a handler Syscall\n", 0);
#endif
    unsigned int time = getTODLO();

    //copia stato dalla old area al pcb del processo corrente
    state_t *oldArea = (state_t *)SYSBK_OLDAREA;
    copyState(oldArea, &(currentPcb->p_s));

    //blocco tempo user, aggiorno kernel
    currentPcb->user_time += time - startTimeUser;
    startTimeKernel = time;

    //incremento il program counter di 4
    currentPcb->p_s.pc_epc += 4;

    //distinzione tra syscall e breakpoint
    if (getCauseField(LEFT_SHIFT_EXCCODE, RIGHT_SHIFT_EXCCODES) == EXCCODE_SYSCALL)
    {   
        //registri a0-a3
        unsigned int a0 = currentPcb->p_s.gpr[3];
        unsigned int a1 = currentPcb->p_s.gpr[4];
        unsigned int a2 = currentPcb->p_s.gpr[5];
        unsigned int a3 = currentPcb->p_s.gpr[6];
        
        //int callScheduler = FALSE; //scelgo se chiamare lo scheduler
        //verifica del tipo di syscall
        switch (a0)
        {
        case GETCPUTIME:
            getCpuTime(a1, a2, a3);
            break;
        case CREATEPROCESS:
            createProcess(a1, a2, a3);
            break;
        case TERMINATEPROCESS:
            //callScheduler = TRUE;
            terminateProcess(a1);
            break;
        case VERHOGEN:
            verhogen((int *)a1);
            break;
        case PASSEREN:
            passeren((int *)a1);
            break;
        case WAITCLOCK:
            //callScheduler = TRUE;
            waitClock();
            break;
        case WAITIO:
            //callScheduler = TRUE;
            IOCommand(a1, a2, a3);
            break;
        case SETTUTOR:
            setTutor();
            break;
        case SPECPASSUP:
            specPassUp(a1, a2, a3);
            break;
        case GETPID:
            getPid(a1, a2);
            break;

        default:
            specPassUpHandler(SYSBK);
            break;
        }
    }
    else
    {
        HALT();
    }
    
    /*if(callScheduler) {
        scheduler();
    }*/

    LDST(&(currentPcb->p_s));

    //fermo tempo kernel qui?

}

void trap_handler(void)
{
#ifdef DEBUG
    termprint("dentro a handler trap\n", 0);
#endif
    //aggiorniamo i tempi
    unsigned int time = getTODLO();
    currentPcb->user_time += time - startTimeUser;
    startTimeKernel = time;
    //ci chiediamo se abbiamo un gestore per le trap da specPassUp
    specPassUpHandler(TRAP);
    //HALT();
}

void tlb_handler(void)
{
#ifdef DEBUG
    termprint("dentro a handler tlb\n", 0);
#endif
    //aggiorniamo i tempi
    unsigned int time = getTODLO();
    currentPcb->user_time += time - startTimeUser;
    startTimeKernel = time;
    //ci chiediamo se abbiamo un gestore per le trap da specPassUp
    specPassUpHandler(TLB);
    //HALT();
}

void interrupt_handler(void)
{
#ifdef DEBUG
    termprint("ricevuto interrupt\n", 0);
#endif
    unsigned int time = getTODLO();

    //copia stato dalla old area al pcb del processo corrente
    state_t *oldArea = (state_t *)INT_OLDAREA;
    copyState(oldArea, &(currentPcb->p_s));

    currentPcb->user_time += time - startTimeUser;
    startTimeKernel = time;

    int causeIP = getCauseField(LEFT_SHIFT_IP, RIGHT_SHIFT_IP);
    //mascheramento del bit del terminal device
    //DA RIVEDERE NON BISOGNA FORZARE IL BIT DELL'INTERVAL
    causeIP = maskBit(causeIP, 0, 7);
    //identificazione del tipo di interrupt

    /*
    potrebbero essere sollevati piu interrupt nello stesso momento, lo switch per gli
    interrupt non servira, bisognera controllare effetivamente quel bit
*/
    // #define CAUSE_IP_GET(cause, int_no) ((cause) & (1 << ((int_no) + 24)))
    //non serve un while perchè vengono gestiti in ordine tutti
    if (PROC_LOCAL_TIMER_LINE == (causeIP & PROC_LOCAL_TIMER_LINE))
        processorLocalTimerInterrupt();

    if (INTERVAL_TIMER_LINE == (causeIP & INTERVAL_TIMER_LINE))
        intervalTimerInterrupt();

    if (DISK_DEVICE_LINE == (causeIP & DISK_DEVICE_LINE))
        devicesInterrupt(INT_DISK);

    if (TAPE_DEVICE_LINE == (causeIP & TAPE_DEVICE_LINE))
        devicesInterrupt(INT_TAPE);

    if (NETWORK_DEVICE_LINE == (causeIP & NETWORK_DEVICE_LINE))
        devicesInterrupt(INT_UNUSED); //network

    if (PRINTER_DEVICE_LINE == (causeIP & PRINTER_DEVICE_LINE))
        devicesInterrupt(INT_PRINTER);

    if (TERMINAL_DEVICES_LINE == (causeIP & TERMINAL_DEVICES_LINE))
        terminalInterrupt();
#ifdef DEBUG
    termprint("mi sono fermato qui dopo la gestione degli interrupt\n", 0);
#endif
    LDST(&currentPcb->p_s);
    //and bit a bit di causeIP e ..._LINE se sono è acceso quel bit di causeIP non noto gli altri bit
}
