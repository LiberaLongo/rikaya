#ifndef _P15TEST_H
#define _P15TEST_H

#define TIME_SCALE *((unsigned int *)0x10000024)
#define RAMBASE *((unsigned int *)0x10000000)
#define RAMSIZE *((unsigned int *)0x10000004)
#define RAMTOP (RAMBASE + RAMSIZE)

void log_process_order(int process);
void test1();
void test2();
void test3();

#endif /*_P15TEST_H*/