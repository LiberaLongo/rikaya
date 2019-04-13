#ifndef UTILS_H
#define UTILS_H

//indirizzi new area
#define SYS_BP_NEW_AREA         0x200003D4
#define TRAP_NEW_AREA           0x200002BC
#define TLB_NEW_AREA            0x200001A4
#define INTERRUPT_NEW_AREA      0x2000008C
//indirizzi old area
#define SYS_BP_OLD_AREA     *((unsigned int *) 0x20000348
#define TRAP_OLD_AREA       *((unsigned int *) 0x20000230
#define TLB_OLD_AREA        *((unsigned int *) 0x20000118
#define INTERRUPT_OLD_AREA  *((unsigned int *) 0x20000000
//framesize
#define FRAMESIZE *((unsigned int *)4096) //4KByte //unsigned int?
//timer
#define PROC_LOCAL_TIMER_LINE 2 //binario !


//funzioni in utils.c
int maskBit(int variabile, int operazione, int bitPosition); //operazione è boolean
void copyState(struct state_t *origin, struct state_t *destination);

#endif //UTILS_H