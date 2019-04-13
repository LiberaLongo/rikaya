#include <umps/types.h>
#include "../header/utils.h"

void sys_bp_handler () {
    //SYS/BP
}

void trap_handler() {
    //Trap
}

void tlb_ha
ndler() {
    //TLB
}

void interrupt_handler() {

    //copiare stato dalla old area al pcb del processo corrente
    struct state_t * oldareapointer = ;
    //struct state_t *newArea = (struct state_t *)area;
    copyState(oldareapointer, pcb);
    //identificare la linea con Cause.Ip (controllo che sia il timer)
    
    //identificare il device se linea > 3
    //case switch
    //acknowledgement del interrupt
    //passare il controllo allo scheduler

}