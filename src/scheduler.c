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
    //code scheduler
}