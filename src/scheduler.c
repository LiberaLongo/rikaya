#include <umps/types.h>
#include "../header/utils.h"
//codice schedule

void timerSetting(int numMilliSeconds ) {
    extern unsigned int* TIME_SCALE;
    //leggi il valore di time scale, non lo sappiamo fare
    unsigned int contentTIMESCALE/* = TIME_SCALE*/;

    // da verificare cosa si ottiene con la moltiplicazione -> unsigned int
    setTIMER( numMilliSeconds * 1000 * contentTIMESCALE ) ;
}

void contextSwitch() {
    //code contextSwitch
}
void scheduler() {
    //aggiornare il currentPcb
    //rimuovere il processo dalla ready_queue
    //fare aging alla ready_queue
    //aggiornare priority alla original_priority
    //riaggiungere il currentProcess alla ready_queue
    //settare il timer
    //ldst
    
}