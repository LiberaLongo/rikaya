#include "../header/syscall.h"
#include "../header/const_rikaya.h"

extern struct pcb_t *currentPcb;

//1
//Quando invocata, la SYS1 restituisce il tempo di esecuzione del processo
//che l’ha chiamata fino a quel momento,
//separato in tre variabili:
//–Il tempo usato dal processo come utente (user)
//–Il tempo usato dal processo come kernel
//(tempi di system call e interrupt relativi al processo)
//–Tempo totale trascorso dalla prima attivazione del processo.
//void SYSCALL(GETCPUTIME, unsigned int *user, unsigned int *kernel, unsigned int *wallclock)
extern unsigned int startTimeKernel;

void getCpuTime(int a1, int a2, int a3)
{
    unsigned int *user = (unsigned int* ) a1;
    unsigned int *kernel = (unsigned int* ) a2;
    unsigned int *wallclock = (unsigned int* ) a3;
    //aggiorniamo i tempi
    currentPcb->kernel_time += getTODLO() - startTimeKernel;
    currentPcb->clock_wall = currentPcb->kernel_time + currentPcb->user_time;
    //deferenziazione
    *user = currentPcb->user_time;
    *kernel = currentPcb->kernel_time;
    *wallclock = currentPcb->wall_clock;
}

//2
//Questa system call crea un nuovo processo come figlio del chiamante.
//Il program counter, lo stack pointer,
//e lo stato sono indicati nello stato iniziale.
//Se la system call ha successo il valore di ritorno è zero altrimenti è -1.
//Se cpid != NULL e la chiamata ha successo
//*cpid contiene l’identificatore del processo figlio (indirizzo del PCB)
//int SYSCALL(CREATEPROCESS, state_t *statep, int priority, void ** cpid)
int createProcess(int a1, int a2, int a3)
{
    //conversione dei registri a1, a2, a3
    state_t *statep = (state_t *)a1;
    int priority = a2;
    void **cpid = (void **)a3; // ?
    struct *pcbChild = *cpid;  

    //se a3 è NULL si alloca un nuovo pcb
    //altrimenti si utilizza l'indirizzo precedente
    if (pcbChild == NULL)
        pcbChild = allocPcb();
    //si copia lo stato statep nel pcb pcbChild
    copyState(statep, pcbChild);
    //pcbChild diventa un nuovo figlio di currentPcb
    insertChild(currentPcb, pcbChild);
    //inserisco nella ready queue
    setProcess(pcbChild, priority);
    return O; // quando -1 ??
}

//3
//Quando invocata, la SYS3 termina il processo
//identificato da pid (il proc. Corrente se pid == 0 o
//NULL) ma non la sua progenie. I processi figli
//vengono adottati dal primo antenato che sia
//marcato come “tutore” (o, nel caso non ce ne
//siano, dal processo in cima all’albero
//genealogico). Il processo da terminare deve
//essere un discendente del processo Corrente.
//– Restituisce 0 se ha successo, -1 per errore
//Int SYSCALL(TERMINATEPROCESS, void ** pid, 0, 0)

//booleano che salva se lo abbiamo trovato
int found = FALSE;
//ricontrollare i return

//cercare il processo pid nei discendenti di p per terminarlo.
void auxSearchPid(struct pcb_t *pid, struct pcb_t *p)
{
    if (p == NULL)
        return ;

    if (pid == p)
    {
        found = TRUE; //se la trova fa found = true, per questo è void
        return ;
    }

    if (!(list_is_last(&p->p_sib, &(p->p_parent)->p_child)))
        //ricorsione sui fratelli
        auxSearchPid(pid, container_of(list_next(&p->p_sib), pcb_t, p_sib));

    if (emptyChild(p))
        return ;
    
    //ricorsione sui figli
    auxSearchPid(pid, container_of(list_next(&p->p_child), pcb_t, p_sib));
}
//cercare il primo tutor disponibile
struct pcb_t *auxLinkTutor(struct pcb_t *p)
{
    struct pcb_t * tutor;
    if (p == NULL)
        return NULL;
    //se il processo elinato è un tutor?
    if (p->tutorFlag == TRUE)
        return p;

    if(p->p_parent == NULL)
        return p;    
    //ricorsione sui padri    
    return auxLinkTutor(container_of((&p->p_parent), pcb_t, p_parent);
}

void terminateProcess(int a1)
{
    void **pid = (void **)a1;          // ?
    struct pcb_t *pcbTerminato = *pid; // -> ?
    found = FALSE;

    struct pcb_t *pcbParent;
    if (pcbTerminato->p_parent == NULL)
        return -1;

    if (pid == NULL)    
        pcbTerminato = currentPcb;
    else
        auxSearchPid(pcbTerminato, currentPcb);

    if(found == FALSE)
        return -1;
        
    struct pcb_t *tutor = auxLinkTutor(pcbTerminato);
    //rimuove il pcbTerminato dalla coda dei processi ready.
    outProcQ(ready_queue_h, pcbTerminato);
    //elimina il processo dalla coda del semaforo su cui era bloccato.
    outBlocked(pcbTerminato);

    if(tutor == NULL) {
        //radice
        struct list_head * iter = pcbTerminato;
        while(iter->p_parent != NULL)
            iter = iter->p_parent;
        

    }
    else {
        //scorrere la lista fei figli 
        list_for_each(pos, head)

    }
    //Rimuove il PCB puntato da pcbTerminato dalla lista dei figli del padre.
    outChild(pcbTerminato);

    //AGGIORNARE TEMPO KERNEL
    
    scheduler();
    return 0;
}

//4
//Operazione di rilascio su un semaforo.
//Il valore del semaforo è memorizzato
//nella variabile di tipo intero passata per indirizzo.
//L’indirizzo della v variabile agisce da identificatore per il semaforo.
//void SYSCALL(VERHOGEN, int *semaddr, 0, 0)
//da chiedere conferma
void verhogen(int a1)
{
    int *semaddr = (int *) a1;
    //
    struct pcb_t * removed = removeBlocked(semaddr);
    if(remove != NULL)
        //inserisco nella readyqueue
        insertProcQ(ready_queue_h);
    //aumento il valore del semaforo per rilasciarlo
    *semaddr += 1;    
}

//5
//Operazione di richiesta di un semaforo.
//Il valore del semaforo è memorizzato nella variabile
//di tipo intero passata per indirizzo.
//L’indirizzo int a a variabile agisce da identificatore per il semaforo.
//void SYSCALL(PASSEREN, int *semaddr, 0, 0)
void passeren(int a1)
{
    int *semaddr = (int *) a1;
    if(*semaddr > 0)
        //entra nel semaforo, quindi può entrare un processo in meno successivamente
        *semaddr -= 1;
    else
        //bloccato
        insertBlocked(semaddr, currentPcb);
}

//6
//Semplicemente, questa system call sospende il processo che la invoca
//fino al prossimo tick del clock di sistema (dopo 100 ms).
//NB: se più processi possono sono sospesi a causa di questa system call,
//devono essere tutti riattivati al prossimo tick.
//void SYSCALL(WAITCLOCK, 0, 0, 0)
void waitClock(void)
{
    insertBlocked(/*semaforoWaitClock*/, currentPcb);
    /*
    domande:
    1. nella wait_clock bisogna sospendere il processo
    che la invoca indistintamente per 100ms
    o fino al prossimo tick del clock(che potrebbe essere minore di 100ms)?
    2. nella getCPUtime se la getTODLO() incrementa il Time of Day
    ogni 100ms quando passo dalla parte kernel alla parte user
    in un context switch di 3ms se il processo non ha ancora fatto
    33 context switch ho il tempo che risulta essere 0
    perchè non sono ancora passati 100ms.
    Devo usare getTODLO ?
    3. quando e come inizializzare il semaforo
    */
}

//7
// Questa system call attiva una operazione di I/O
//copiando parametro command nel campo
//comando del registro del dispositivo indicato
//come puntatore nel secondo argomento.
// L’operazione è bloccante, quindi il chiamante
//viene sospeso sino alla conclusione del comando.
//Il valore ritornato è il contenuto del registro di
//status del dispositivo.
//int SYSCALL(IOCOMMAND, unsigned int command, unsigned int *register, FALSE)
void IOCommand(int a1, int a2, int a3)
{
    unsigned int command = (unsigned int ) a1;
    unsigned int * IOregister = (unsigned int *) a2;
    int write = a3;
    /*
    time = getTODLO();
    ora = time - time % (1000000*100)
    if(variabile precedente < ora)
        variabile precedente = time - time%
    */
}

//8
//Indica al kernel che il processo che la invoca deve agire da tutor
//per i processi discendenti che dovessero trovarsi orfani
//e che quindi diventeranno suoi figli.
//Si può implementare in diversi modi;
//per esempio, aggiungendo un campo nel pcb che marchi i tutor.
//void SYSCALL(SETTUTOR, 0, 0, 0)
void setTutor(void)
{
    //
}

//9
//Questa chiamata registra quale handler di livello superiore
//debba essere attivato in caso di trap di
//Syscall/breakpoint (type=0), TLB (type=1) o Program trap (type=2).
//Il significato dei parametri old
//e new è lo stesso delle aree old e new gestite dal codice della ROM:
//quando avviene una trap da passare al gestore lo stato del processo
//che ha causato la trap viene posto nell’area old e viene caricato
//o stato presente nell’area new.
//La system call deve essere richiamata una sola volta per tipo.
//Se la system cint a1, int a2, int a3ha successo restituisce 0, altrimenti -1.
//int SYSCALL(SPECPASSUP, int type, state_t *old, state_t *new)
void specPassUp(int a1, int a2, int a3)
{
    //
}

//10
//Questa system call assegna il l’identificativo
//del processo corrente a *pid (se pid != NULL)
//e l’identiint a1, int a2tivo del processo genitore a *ppid (se ppid != NULL)
//Void SYSCALL(GETPID, void ** pid, void ** ppid, 0)
void getPid(int a1, int a2)
{
    //
}

//SYSCALL > 10
//Devono essere inoltrati al gestore di livello superiore se presente
//(i.e. se è stato specificato da una SYS10),
//altrimenti causano la terminazione del processo.
//Stesso dicasi per le eccezioni di tipo TRAP