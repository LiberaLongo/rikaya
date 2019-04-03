//codice inizializzazione
#include <umps/types.h>
#include "../header/utils.h"

/*
Inizializzazione del sistema
• Per ogni New Area:
1. Inizializzare il PC all’indirizzo dell’handler del
nucleo che gestisce quell’eccezione. (?)
2. Inizializzare $SP a RAMPTOP
3. Inizializzare il registro di status:
    - mascherare interrupt
    - disabilitare virtual memory
    - settare kernel mode ON
    - abilitare un timer
*/
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
COME MASCHERARE UN BIT IN UN INTERO
Per settare a 1 il bit alla posizione bitPosition menosignificativo
intValue |= 1 << bitPosition;
Per settare a 0 il bit alla posizione bitPosition menosignificativo
intValue &= ~(1 << bitPosition);
*/

void initNewArea( memaddr area , memaddr handler ) {
    state_t *newArea = (state_t*) area ;
    //1. Inizializzare il PC all’indirizzo dell’handler del
        //nucleo che gestisce quell’eccezione
    newArea->pc_epc = handler;
    //$SP è gpr[28] (slide 16 pag 4 manuale)
    //2. Inizializzare $SP a RAMPTOP
    newArea->gpr[26] = RAMTOP; // ?
    //3. Inizializzare il registro di status:
    //- mascherare interrupt
    newArea->
    //- disabilitare virtual memory
    //- settare kernel mode ON
    //- abilitare un timer

    newArea->
    //SIAMO ARRIVATI QUI!
}

void initialization() {
    //(memaddr) ..._handler è l'indirizzo della funzione (handler) che gestisce l'eccezione
    //SYS/BP
    initNewArea(SYS_BP_NEW_AREA, (memaddr) sys_bp_handler );
    //Trap
    initNewArea(TRAP_NEW_AREA, (memaddr) trap_handler );
    //TLB
    initNewArea(TLB_NEW_AREA, (memaddr) tlb_handler );
    //Interrupt
    initNewArea(INTERRUPT_NEW_AREA, (memaddr) interrupt_handler );
}