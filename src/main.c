#include <umps/types.h>
#include "../header/init.h"
#include "../header/pcb.h"
#include "../header/scheduler.h"

#include "../header/p15test.h"

int main()
{
    //Inizializzazione delle newArea e della freePcb list
    initialization();
    //Instanziare i 3 pcb e lo stato dei 3 processi di test e inserimento nella readyQueue
    setProcess(allocPcb(), (memaddr)test1, 1);
    setProcess(allocPcb(), (memaddr)test2, 2);
    setProcess(allocPcb(), (memaddr)test3, 3);
    //lancio del primo processo
    scheduler();

    return 0;
}
