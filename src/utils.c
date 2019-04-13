#include <umps/types.h>
#include "../header/utils.h"

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

//copia stato da origine a destinazione
void copyState(struct state_t *origin, struct state_t *destination)
{
    //setta uno a uno i campi del processor state
    //entry_hi;
    destination->entry_hi = origin->entry_hi;
    //cause;
    destination->cause = origin->cause;
    //status;
    destination->status = origin->status;
    //pc_epc;
    destination->pc_epc = origin->pc_epc;
    //gpr[STATE_GPR_LEN];
    for (int i = 0; i < STATE_GPR_LEN; i++)
        destination->gpr[i] = origin->gpr[i];
    //hi;
    destination->hi = origin->hi;
    //lo;
    destination->lo = origin->lo;
}