#include <umps/types.h>
#include "../header/init.h"
#include "../header/pcb.h"
#include "../header/scheduler.h"

#include "../header/p15test.h"

unsigned int memTest1 = (memaddr)test1;
unsigned int memTest2 = (memaddr)test2;
unsigned int memTest3 = (memaddr)test3;
int main()
{
    initialization();
    //termprint("inizializzazione completata\n", 0);
    

    //• Instanziare il PCB e lo stato dei 3 processi di test
    

    setProcess(allocPcb(), (memaddr)test1, 1);
    //termprint("inserito il primo processo\n",0);
    setProcess(allocPcb(), (memaddr)test2, 2);
    //termprint("inserito secondo processo\n", 0);
    setProcess(allocPcb(), (memaddr)test3, 3);
    //termprint("inserito terzo processo\n", 0);
    //termprint("lancio del primo processo\n", 0);


    scheduler();

    return 0;
}
