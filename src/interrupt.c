#include "../header/interrupt.h"

void processorLocalTimerInterrupt(void)
{

#ifdef DEBUG
    termprint("processor local timer interrupt\n",0);
#endif
    //acknowledgement(risettaggio del timer) viene fatto all'interno dello scheduler
    scheduler();
}

void intervalTimerInterrupt()
{
#ifdef DEBUG
    termprint("Interval timer interrupt\n",0);
#endif
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
#ifdef DEBUG
    termprint("Device interrupt\n",0);
#endif
    //3 4 5 6 7 -> 0 1 2 3 4
    line = line - 3;

    unsigned int *mem = (unsigned int *)PENDING_BITMAP_START;
    unsigned int deviceBitMap = *(mem + line * WORD_SIZE);

    //calcolare l'indirizzo del device corrispondente
    unsigned int *memDev = (unsigned int *)DEV_REGS_START;
    //guardiamo solo i primi 8 bit
    deviceBitMap = deviceBitMap << 24;
    deviceBitMap = deviceBitMap >> 24;
    //finchè non ci sono tutti i device gestiti
    //non serve un while perchè vengono gestiti in ordine tutti
    for (int i = 0; i < DEV_PER_INT - 1; i++)
    {
        //se il bit i-esimo (calcolato con 2^i) è 1 pende un interrupt di questo device
        if (pot(2, i) == (deviceBitMap & pot(2, i))) //device i
        {
            verhogen(&deviceSem[i + line * DEV_PER_INT]);
            //acknowledgement
            //((linea*8 + nuimero dispositivo (i)) * 16) + primoindirizzo
            unsigned int *devAddress = (unsigned int *)(memDev + (line * DEV_PER_INT + i) * 16);
            //settare  campo command di quel indirizzo a DEV_C_ACK
            unsigned int *command = devAddress + 0x4;
            *command = DEV_C_ACK;
            //riazzerare l'interrupt (in teoria già fatto dal ack)
            //tempi?
        }
    }
}

void terminalInterrupt(void)
{
#ifdef DEBUG
    termprint("terminal interrupt\n",0);
#endif
    //interrupt terminal line bitmap
    unsigned int *mem = (unsigned int *)TERMINAL_BITMAP;
    unsigned int terminalBitMap = *(mem);

    //calcolare l'indirizzo del device corrispondente
    unsigned int *memTerm = (unsigned int *)DEV_REGS_START;
    //guardiamo solo i primi 8 bit
    terminalBitMap = terminalBitMap << 24;
    terminalBitMap = terminalBitMap >> 24;

    for (int i = 0; i < DEV_PER_INT - 1; i++)
    {
        if (pot(2, i) == (terminalBitMap & pot(2, i))) //device i
        {
            //((linea*8 + nuimero dispositivo (i)) * 16) + primoindirizzo
            unsigned int *termAddress = (unsigned int *)(memTerm + ((INT_TERMINAL - 3) * DEV_PER_INT + i) * 16);
            unsigned int *command;
            //trasmissione scrittura (ha priorità più alta della lettura?)
            if ((*(termAddress + 0x8) & 0xFF) == DEV_TTRS_S_CHARTRSM)
            {
                //trasmissione scrittura//
                verhogen(&deviceSem[i + (INT_TERMINAL + 1) * 8]);
                command = termAddress + 0xc;
                *command = DEV_C_ACK;
            }
            else if ((*(termAddress + 0x8) & 0xFF) == DEV_TRCV_S_CHARRECV)
            {
                //ricezione lettura
                verhogen(&deviceSem[i + INT_TERMINAL * 8]);
                command = termAddress + 0x4;
                *command = DEV_C_ACK;
            }
        }
    }
}