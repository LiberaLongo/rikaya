#ifndef UTILS_H
#define UTILS_H
#include <umps/libumps.h>
//indirizzi new area
#define SYS_BP_NEW_AREA         0x200003D4
#define TRAP_NEW_AREA           0x200002BC
#define TLB_NEW_AREA            0x200001A4
#define INTERRUPT_NEW_AREA      0x2000008C
//indirizzi old area
#define SYS_BP_OLD_AREA         0x20000348
#define TRAP_OLD_AREA           0x20000230
#define TLB_OLD_AREA            0x20000118
#define INTERRUPT_OLD_AREA      0x20000000

//a che serve *((unsigned int *) 0x ... ) ???

//framesize
#define FRAMESIZE *((unsigned int *)4096) //4KByte //unsigned int?
//timer
#define PROC_LOCAL_TIMER_LINE 2 //binario !

//CauseIP
#define LEFT_SHIFT_IP 16
#define RIGHT_SHIFT_IP 8
//Cause.ExcCode
#define LEFT_SHIFT_EXCCODE 25
#define RIGHT_SHIFT_EXCCODES 2

//exccode syscall
#define EXCCODE_SYSCALL 8

//codici syscall in syscall.h

//funzioni in utils.c
struct pcb_t;

int maskBit(int variabile, int operazione, int bitPosition); //operazione è boolean
void copyState( state_t *origin, struct pcb_t *destination);

#endif //UTILS_H

/*
#define reg_at gpr[0]
#define reg_v0 gpr[1]
#define reg_v1 gpr[2]
#define reg_a0 gpr[3]
#define reg_a1 gpr[4]
#define reg_a2 gpr[5]
#define reg_a3 gpr[6]
#define reg_t0 gpr[7]
#define reg_t1 gpr[8]
#define reg_t2 gpr[9]
#define reg_t3 gpr[10]
#define reg_t4 gpr[11]
#define reg_t5 gpr[12]
#define reg_t6 gpr[13]
#define reg_t7 gpr[14]
#define reg_s0 gpr[15]
#define reg_s1 gpr[16]
#define reg_s2 gpr[17]
#define reg_s3 gpr[18]
#define reg_s4 gpr[19]
#define reg_s5 gpr[20]
#define reg_s6 gpr[21]
#define reg_s7 gpr[22]
#define reg_t8 gpr[23]
#define reg_t9 gpr[24]
#define reg_gp gpr[25]
#define reg_sp gpr[26]
#define reg_fp gpr[27]
#define reg_ra gpr[28]
#define reg_HI gpr[29]
#define reg_LO gpr[30]
*/