#include <umps/types.h>
#include "../header/init.h"
#include "../header/pcb.h"
#include "../header/scheduler.h"

void test(); //indico al linker di linkare questo test con il test di p2test.c

int main()
{
#ifdef DEBUG
    termprint("dentro a main1o\n", 0);
#endif

    //Inizializzazione delle newArea e della freePcb list
    initialization();

#ifdef DEBUG
    termprint("inizializzazione fatta\n", 0);
#endif

    //Instanziare i pcb e lo stato del processo di test e inserimento nella readyQueue
    setFirstProcess(allocPcb(), (memaddr)test, 1);

#ifdef DEBUG
    termprint("settato primo processo\n", 0);
#endif
    //lancio del primo processo
    scheduler();

    return 0;
}
