#include <umps/types.h>
#include "../header/init.h"
#include "../header/pcb.h"
#include "../header/scheduler.h"

int main()
{
    initialization();

    //• Instanziare il PCB e lo stato dei 3 processi di test
    setProcess(allocPcb(), (memaddr)test1, 1);
    setProcess(allocPcb(), (memaddr)test2, 2);
    setProcess(allocPcb(), (memaddr)test3, 3);

    currentPcb = NULL;

    scheduler();

    return 0;
}
