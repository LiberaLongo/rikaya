//codice inizializzazione
#include <umps/types.h>
#include "../header/utils.h"

LIST_HEAD(ready_queue);
struct list_head *ready_queue_h = &ready_queue;
//puntatore al processo corrente
struct pcb_t * currentPcb;

/*
// Processor state
typedef struct state {
    unsigned int entry_hi;
    unsigned int cause;
    unsigned int status;
    unsigned int pc_epc;
    unsigned int gpr[STATE_GPR_LEN];
    unsigned int hi;
    unsigned int lo;
} state_t;
*/

/*
#define reg_sp gpr[26]
*/

void initNewArea(memaddr area, memaddr handler)
{
    struct state_t *newArea = (struct state_t *)area;
    //1. Inizializzare il PC all’indirizzo dell’handler del
    //nucleo che gestisce quell’eccezione
    newArea->pc_epc = handler;
    //$SP è gpr[28] (slide 16 pag 4 manuale)
    //2. Inizializzare $SP a RAMPTOP
    newArea->gpr[26] = RAMTOP; // ?
    //3. Inizializzare il registro di status:
    //- mascherare interrupt -> IEc 0
    newArea->status = maskBit(newArea->status, 0, 0);
    //- disabilitare virtual memory -> VMc 0
    newArea->status = maskBit(newArea->status, 0, 24);
    //- settare kernel mode ON -> KUc 0
    newArea->status = maskBit(newArea->status, 0, 1);
    //- abilitare un timer -> TE 1
    newArea->status = maskBit(newArea->status, 1, 27);
    //SIAMO ARRIVATI QUI!

    /*
    IEc bit pos 0 bit per abilitare gli interrupt -> 0 disabilitati, 1 va a vedere status.IM
    KUc bit pos 1 kernel mode da settare a 0 -> 0 kernel mode, 1 kernel mode
    VMc bit pos 24 virtual memory -> 0 = vitual memory off , 1 virtual memory on
    TE bit pos 27 timer -> 0 non gerenera interrupt, 1 genera interrupt
    processor local timer
    (partendo il conto da 0)
    */
}

void setProcess(struct pcb_t *pcb, memaddr function, int priority)
{
    if (pcb != NULL)
    {
        //- Interrupt abilitati -> IEc 1
        pcb->p_s.status = maskBit(pcb->p_s.status, 1, 0);
        //- Virtual Memory OFF -> VMc 0
        pcb->p_s.status = maskBit(pcb->p_s.status, 0, 24);
        //- Processor Local Timer abilitato -> TE 1
        pcb->p_s.status = maskBit(pcb->p_s.status, 1, 27);
        //- Kernel-Mode ON -> KUc 0
        pcb->p_s.status = maskBit(pcb->p_s.status, 0, 1);
        //- $SP=RAMTOP-FRAMESIZE*n
        pcb->p_s.gpr[26] = RAMTOP - FRAMESIZE * priority;
        //- priorita’ = n
        pcb->p_s.priority = priority;
        //    ----> original_priority = n
        pcb->p_s.original_priority = priority;
        //- Settare PC all’entry-point dei test
        //pstate.pc_epc=(memaddr) testn
        pcb->p_s.pc_epc = function;
        //• Inseririre i processi nella Ready Queue*/
        insertProcQ(ready_queue_h, pcb);
    }
}

void initialization(void)
{
    //(memaddr) ..._handler è l'indirizzo della funzione (handler) che gestisce l'eccezione
    //SYS/BP
    initNewArea(SYS_BP_NEW_AREA, (memaddr)sys_bp_handler);
    //Trap
    initNewArea(TRAP_NEW_AREA, (memaddr)trap_handler);
    //TLB
    initNewArea(TLB_NEW_AREA, (memaddr)tlb_handler);
    //Interrupt
    initNewArea(INTERRUPT_NEW_AREA, (memaddr)interrupt_handler);

    /*Inizializzare strutture dati di Phase1 (solo i pcb):
    initPcbs()*/
    initPcbs();
    /*• Inizializzare variabili del kernel:
    Per ora, solo la lista dei processi
    LIST_HEAD(ready_queue);*/
    INIT_LIST_HEAD(ready_queue_h);

    //• Instanziare il PCB e lo stato dei 3 processi di test
    struct pcb_t *pcb1 = allocPcb();
    struct pcb_t *pcb2 = allocPcb();
    struct pcb_t *pcb3 = allocPcb();

    setProcess(pcb1, (memaddr)test1, 1);
    setProcess(pcb2, (memaddr)test2, 2);
    setProcess(pcb3, (memaddr)test3, 3);

    currentPcb = NULL;

    //• Inseririre i processi nella Ready Queue*/
}