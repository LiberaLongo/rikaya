
#include "../header/handler.h"

extern pcb_t *currentPcb;
/*Registri a0-a3
#define reg_a0 gpr[3]
#define reg_a1 gpr[4]
#define reg_a2 gpr[5]
#define reg_a3 gpr[6]
*/
void sys_bp_handler(void)
{
    //copia stato dalla old area al pcb del processo corrente
    state_t *oldArea = (state_t *)SYS_BP_OLD_AREA;
    copyState(oldArea, currentPcb);

    //distinzione tra syscall e breakpoint
    if (getCauseField(LEFT_SHIFT_EXCCODE, RIGHT_SHIFT_EXCCODES) == EXCCODE_SYSCALL)
    {
        int a0 = currentPcb->p_s.gpr[3];
        int a1 = currentPcb->p_s.gpr[4];
        int a2 = currentPcb->p_s.gpr[5];
        int a3 = currentPcb->p_s.gpr[6];

        int ret = 0; //ritorno
        //verifica del tipo di syscall
        switch (a0)
        {
        case GETCPUTIME:
            getCpuTime(a1, a2, a3);
            break;
        case CREATEPROCESS:
            ret = createProcess(a1, a2, a3);            
            if(ret == -1)
                PANIC();
            break;
        case TERMINATEPROCESS:
            terminateProcess(a1);
            break;
        case VERHOGEN:
            verhogen(a1);
            break;
        case PASSEREN:
            passeren(a1);
            break;
        case WAITCLOCK:
            waitClock();
            break;
        case IOCOMMAND:
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
            PANIC();
            break;
        }
    }
    else
    {
        PANIC();
    }
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
    //copia stato dalla old area al pcb del processo corrente
    state_t *oldArea = (state_t *)INTERRUPT_OLD_AREA;
    copyState(oldArea, currentPcb);

    int causeIP = getCauseField(LEFT_SHIFT_IP, RIGHT_SHIFT_IP);
    //mascheramento del bit del terminal device
    causeIP = maskBit(causeIP, 0, 7);
    //identificazione del tipo di interrupt
    switch (causeIP)
    {
    case PROC_LOCAL_TIMER_LINE:
        timerInterruptManagement();
        break;

    default:
        //per la fase 2 verranno differenziati gli altri casi
        PANIC();
        break;
    }
}