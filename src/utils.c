#include <umps/types.h>
#include "../header/utils.h"
#include "../header/pcb.h"

/*
COME MASCHERARE UN BIT IN UN INTERO
Per settare a 1 il bit alla posizione bitPosition menosignificativo
intValue |= 1 << bitPosition;
Per settare a 0 il bit alla posizione bitPosition menosignificativo
intValue &= ~(1 << bitPosition);
*/
int maskBit(int variabile, int operazione, int bitPosition)
{
    int result = variabile;
    //se operazione = 0 setto il bit a 0
    if (operazione == 0)
        result &= ~(1 << bitPosition);
    //se operazione = 1 setto il bit a 1
    if (operazione == 1)
        result |= 1 << bitPosition;
    return result;
}

//copia stato da origine (old area) a destinazione (currentPcb)
void copyState(struct state_t* origin, struct pcb_t* destination)
{
    //setta uno a uno i campi del processor state
    //entry_hi;
    destination->p_s.entry_hi = origin->entry_hi;
    //cause;
    destination->p_s.cause = origin->cause;
    //status;
    destination->p_s.status = origin->status;
    //pc_epc;
    destination->p_s.pc_epc = origin->pc_epc;
    //gpr[STATE_GPR_LEN];
    for (int i = 0; i < STATE_GPR_LEN; i++)
        destination->p_s.gpr[i] = origin->gpr[i];
    //hi;
    destination->p_s.hi = origin->hi;
    //lo;
    destination->p_s.lo = origin->lo;
}

int getCauseField(int leftShift, int rightShift) {
    
    int cause = getCAUSE();
    //sposto cause a più significativo eliminando davanti a lui
    cause << leftShift; 
    //sposto cause a meno significativo eliminando inseriti prima + dietro
    cause >> (rightShift + leftShift);
    return cause;
}
    //esempio:
    //se sono ad x tutti quelli che mi servono e sono tutti a 1 gli altri
    //  cause =     1111 1111   1111 1111   xxxx xxxx   1111 1111
    //  cause << 16
    //  cause =>    xxxx xxxx   1111 1111   0000 0000   0000 0000
    //  cause >> 24
    //  cause =>    0000 0000   0000 0000   0000 0000   xxxx xxxx
