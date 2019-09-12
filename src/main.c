#include <umps/types.h>
#include "../header/init.h"
#include "../header/pcb.h"
#include "../header/scheduler.h"

void test(); //indico al linker di linkare questo test con il test di p2test.c

int main()
{
    //Inizializzazione delle newArea e della freePcb list
    initialization();
    
    //Instanziare i pcb e lo stato del processo di test e inserimento nella readyQueue
    setFirstProcess(allocPcb(), (memaddr)test, 1);
    
    //lancio del primo processo
    scheduler();

    return 0;
}
