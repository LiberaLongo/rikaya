#include "../header/utils.h"

//funzoione per settare a 1 o 0 il bit desiderato (bitPosition)
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
void copyState(state_t *origin, struct pcb_t *destination)
{
    //setta uno a uno i campi del processor state
    destination->p_s.entry_hi = origin->entry_hi;
    destination->p_s.cause = origin->cause;
    destination->p_s.status = origin->status;
    destination->p_s.pc_epc = origin->pc_epc;

    for (int i = 0; i < STATE_GPR_LEN; i++)
        destination->p_s.gpr[i] = origin->gpr[i];

    destination->p_s.hi = origin->hi;
    destination->p_s.lo = origin->lo;
}

//funzione che restituisce i bit del campo Cause desiderato(causeIp, causeExccode)
int getCauseField(int leftShift, int rightShift)
{
    unsigned int cause = getCAUSE();
    //si spostano i bit desiderati alla posizione piu significativa
    cause = cause << leftShift;
    //si spostano i bit desiderati alla posizione meno significativa
    cause = cause >> (rightShift + leftShift);
    return cause;
}

void setProcessorLocalTimer(int numMilliSeconds)
{
    //settaggio del timer in ms
    setTIMER(numMilliSeconds * 1000 * TIME_SCALE);
}

void setIntervalTimer(int numMilliseconds) {
    unsigned int * interval_timer = (unsigned int *)INTERVAL_TIMER_MEM;
    *interval_timer = numMilliseconds * 1000 * TIME_SCALE;
}
//ridare il controllo al processo senza chiamare lo scheduler
void incrementProgramCounter(void) {
    //incremento il program counter di 4.
}