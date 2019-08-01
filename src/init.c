#include "../header/init.h"

LIST_HEAD(ready_queue);
struct list_head *ready_queue_h = &ready_queue;

void initNewArea(memaddr area, memaddr handler)
{

    state_t *newArea = (state_t *)area;
    //handler della relativa eccezione
    newArea->pc_epc = handler;
    //Inizializzazione di $SP(gpr[26])
    newArea->gpr[26] = RAMTOP;
    //Inizializzare il registro di status
    //-IM[processor local timer] 1 abilita gli interrupt del processor local timer
    newArea->status = maskBit(newArea->status, 1, 9);
    //- abilitare un timer -> TE 1
    newArea->status = maskBit(newArea->status, 1, 27);
}

void setProcess(struct pcb_t *pcb, memaddr function, int priority)
{
    if (pcb != NULL)
    {
        //- Interrupt abilitati -> IEp 1
        pcb->p_s.status = maskBit(pcb->p_s.status, 1, 2);
        
        //- IM[processor local timer] 1
        pcb->p_s.status = maskBit(pcb->p_s.status, 1, 9);
        
        //- Virtual Memory OFF -> VMp 0
        pcb->p_s.status = maskBit(pcb->p_s.status, 0, 25);
        
        //- Processor Local Timer abilitato -> TE 1
        pcb->p_s.status = maskBit(pcb->p_s.status, 1, 27);
        
        //- Kernel-Mode ON -> KUp 0
        pcb->p_s.status = maskBit(pcb->p_s.status, 0, 3);
        
        //- $SP(gpr[26])
        pcb->p_s.gpr[26] = RAMTOP - FRAMESIZE * priority;
        
        //- settaggio delle priorita’
        pcb->priority = priority;
        pcb->original_priority = priority;

        //settaggio del camppo tutor a false
        pcb->tutorFlag = 0;
        //- PC all’entry-point dei test
        pcb->p_s.pc_epc = function;

        //-settaggio dei time
        pcb->user_time = 0;
	    pcb->kernel_time = 0;
	    pcb->clock_wall = 0;
        //- Inseririmento i processi nella Ready Queue
        insertProcQ(ready_queue_h, pcb);
    }
}

void initialization(void)
{
    //(memaddr)...handler è l'indirizzo della funzione (handler) che gestisce l'eccezione

    //Inizializzazione delle new Area
    initNewArea(SYS_BP_NEW_AREA, (memaddr)sys_bp_handler);
    initNewArea(TRAP_NEW_AREA, (memaddr)trap_handler);
    initNewArea(TLB_NEW_AREA, (memaddr)tlb_handler);
    initNewArea(INTERRUPT_NEW_AREA, (memaddr)interrupt_handler);

    initPcbs();
    initASL();
    

    //Inizializzazione della readyQueue
    INIT_LIST_HEAD(ready_queue_h);
}