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
void copyState(state_t *origin, state_t *destination)
{
    //setta uno a uno i campi del processor state
    destination->entry_hi = origin->entry_hi;
    destination->cause = origin->cause;
    destination->status = origin->status;
    destination->pc_epc = origin->pc_epc;

    for (int i = 0; i < STATE_GPR_LEN; i++)
        destination->gpr[i] = origin->gpr[i];

    destination->hi = origin->hi;
    destination->lo = origin->lo;
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
    unsigned int * interval_timer = (unsigned int *)BUS_INTERVALTIMER;
    *interval_timer = numMilliseconds * 1000 * TIME_SCALE;
}
//ridare il controllo al processo senza chiamare lo scheduler
void incrementProgramCounter(void) {
    //incremento il program counter di 4.
}

int pot(int a, int b){
    if(b == 0)
        return 1;
    if(b == 1)
        return a;
    else
        return a * pot(a, (b-1));
}

#ifdef DEBUG

#include "../header/const_rikaya.h"

typedef unsigned int devreg;

#define CHAROFFSET 8
#define PRINTCHR 2
#define BUSY 3
#define DEVREGSIZE 16
#define DEVREGLEN 4
#define READY 1
#define TRANSMITTED 5

/******************************************************************************
 * I/O Routines to write on a terminal
 ******************************************************************************/

/* This function returns the terminal transmitter status value given its address */
devreg termstat(memaddr *stataddr)
{
    return ((*stataddr) & STATUSMASK);
}

/* This function prints a string on specified terminal and returns TRUE if 
 * print was successful, FALSE if not   */
unsigned int termprint(char *str, unsigned int term)
{

    memaddr *statusp;
    memaddr *commandp;

    devreg stat;
    devreg cmd;

    unsigned int error = FALSE;

    if (term < DEV_PER_INT)
    {
        /* terminal is correct */
        /* compute device register field addresses */
        statusp = (devreg *)(TERM0ADDR + (term * DEVREGSIZE) + (TRANSTATUS * DEVREGLEN));
        commandp = (devreg *)(TERM0ADDR + (term * DEVREGSIZE) + (TRANCOMMAND * DEVREGLEN));

        /* test device status */
        stat = termstat(statusp);
        if ((stat == READY) || (stat == TRANSMITTED))
        {
            /* device is available */

            /* print cycle */
            while ((*str != '\0') && (!error))
            {
                cmd = (*str << CHAROFFSET) | PRINTCHR;
                *commandp = cmd;

                /* busy waiting */
                while ((stat = termstat(statusp)) == BUSY)
                    ;

                /* end of wait */
                if (stat != TRANSMITTED)
                {
                    error = TRUE;
                }
                else
                {
                    /* move to next char */
                    str++;
                }
            }
        }
        else
        {
            /* device is not available */
            error = TRUE;
        }
    }
    else
    {
        /* wrong terminal device number */
        error = TRUE;
    }

    return (!error);
}

#endif