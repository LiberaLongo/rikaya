#include "../header/interrupt.h"

void processorLocalTimerInterrupt(void)
{
    //acknowledgement(risettaggio del timer) viene fatto all'interno dello scheduler
    scheduler();
}

void intervalTimerInterrupt()
{
    //rilascio tutti i processi bloccati per la WAITCLOCK
    while (headBlocked(&deviceSem[CLOCK_SEM]) != NULL)
    {
        verhogen(&deviceSem[CLOCK_SEM]);
    }
    //semaforo di WAITCLOCK
    setIntervalTimer(100);
    //svuoto la lista di processi
    //resettare interval timer
    //incrementare il program counter di 4(stesso processo)
    //LDST currentPcb->newrea
    //ripartire il processo corrente
}

void devicesInterrupt(int line)
{
    unsigned int *mem = (unsigned int *)PENDING_BITMAP_START;
    unsigned int deviceBitMap = *(mem + line * WORD_SIZE);
    //guardiamo solo i primi 8 bit
    deviceBitMap = deviceBitMap << 24;
    deviceBitMap = deviceBitMap >> 24;
    //finchè non ci sono tutti i device gestiti
    //non serve un while perchè vengono gestiti in ordine tutti
    for (int i = 0; i < DEV_LINE - 1; i++)
    {
        //se il bit i-esimo (calcolato con 2^i) è 1 pende un interrupt di questo device
        if (pot(2, i) == (deviceBitMap & pot(2, i))) //device i
        {
            verhogen(&deviceSem[i + line * 8]);
            //acknowledgement
            //riazzerare l'interrupt
        }
    }
}

void terminalInterrupt(void)
{
    unsigned int *mem = (unsigned int *)0x1000.004c;
    unsigned int deviceBitMap = *(mem);
    //guardiamo solo i primi 8 bit
    deviceBitMap = deviceBitMap << 24;
    deviceBitMap = deviceBitMap >> 24;

    for (int i = 0; i < DEV_LINE - 1; i++)
    {
        if (pot(2, i) == (deviceBitMap & pot(2, i))) //device i
        {
        //ricezione
        verhogen(&deviceSem[i + INT_TERMINAL * 8]);
        //trasmissione
        verhogen(&deviceSem[i + (INT_TERMINAL+1) * 8]);
        }
    }
}