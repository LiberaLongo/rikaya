#include "../header/syscall.h"

extern struct pcb_t * currentPcb;

//1
//Quando invocata, la SYS1 restituisce il tempo di esecuzione del processo
//che l’ha chiamata fino a quel momento, 
//separato in tre variabili:–Il tempo usato dal processo come utente (user)
//–Il tempo usato dal processo come kernel
//(tempi di system call e interrupt relativi al processo)
//–Tempo totale trascorso dalla prima attivazione del processo.
//void SYSCALL(GETCPUTIME, unsigned int *user, unsigned int *kernel, unsigned int *wallclock)
void getCpuTime(int a1, int a2, int a3) {
    //
}

//2
//Questa system call crea un nuovo processo come figlio del chiamante.
//Il program counter, lo stack pointer,
//e lo stato sono indicati nello stato iniziale.
//Se la system call ha successo il valore di ritorno è zero altrimenti è -1.
//Se cpid != NULL e la chiamata ha successo
//*cpid contiene l’identificatore del processo figlio (indirizzo del PCB)
//int SYSCALL(CREATEPROCESS, state_t *statep, int priority, void ** cpid)
int createProcess(int a1, int a2, int a3) {
    //conversione dei registri a1, a2, a3
    state_t *statep = (state_t *) a1;
    int priority = a2;
    void **cpid = (void **) a3; // ?
    struct *pcbChild = *cpid; // -> ?

    //se a3 è NULL si alloca un nuovo pcb
    //altrimenti si utilizza l'indirizzo precedente
    if(pcbChild == NULL)
        pcbChild = allocPcb();
    //si copia lo stato statep nel pcb pcbChild
    copyState(statep, pcbChild);
    //pcbChild diventa un nuovo figlio di currentPcb
    insertChild(currentPcb, pcbChild);

    return O; // quando -1 ?? 
}

//3
//rimozione del processo corrente dalla readyQueue
//e caricamento del print a1so successivo
//Int SYSCALL(TERMINATEPROCESS, void ** pid, 0, 0)
void terminateProcess(int a1)
{
    outProcQ(ready_queue_h, currentPcb);
    scheduler();
}

//4
//Operazione di rilascio su un semaforo. 
//Il valore del semaforo è memorizzato
//nella variabile di tipo intero passata per indirizzo.
//L’indirizzo int a1a variabile agisce da identificatore per il semaforo.
//void SYSCALL(VERHOGEN, int *semaddr, 0, 0)
void verhogen(int a1) {
    //
}

//5
//Operazione di richiesta di un semaforo.
//Il valore del semaforo è memorizzato nella variabile
//di tipo intero passata per indirizzo.
//L’indirizzo int a1a variabile agisce da identificatore per il semaforo.
//void SYSCALL(PASSEREN, int *semaddr, 0, 0)
void passeren(int a1) {
    //
}

//6
//Semplicemente, questa system call sospende il processo che la invoca
//fino al prossimo tick del clock di sistema (dopo 100 ms).
//NB: se più processi possono sono sospesi a causa di questa system call,
//devono essere tutti riattivati al prossimo tick.
//void SYSCALL(WAITCLOCK, 0, 0, 0)
void waitClock(void) {
    //
}

//7
//Questa system call attiva una operazione di I/O copiando parametro command
//nel campo comando del registro del dispositivo indicato come puntatore
//nel secondo argomento.
//L’operazione è bloccante,
//quindi il chiamante viene sospeso sino alla conclusione del comando.
//Il valore ritint a1, int a2to è il contenuto del registro di status del dispositivo
//int SYSCALL(IOCOMMAND, unsigned int command, unsigned int *register, 0)
void IOCommand(int a1, int a2, int a3) {
    //
}

//8
//Indica al kernel che il processo che la invoca deve agire da tutor
//per i processi discendenti che dovessero trovarsi orfani
//e che quindi diventeranno suoi figli.
//Si può implementare in diversi modi;
//per esempio, aggiungendo un campo nel pcb che marchi i tutor.
//void SYSCALL(SETTUTOR, 0, 0, 0)
void setTutor(void) {
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
void specPassUp(int a1, int a2, int a3) {
    //
}

//10
//Questa system call assegna il l’identificativo
//del processo corrente a *pid (se pid != NULL)
//e l’identiint a1, int a2tivo del processo genitore a *ppid (se ppid != NULL)
//Void SYSCALL(GETPID, void ** pid, void ** ppid, 0)
void getPid(int a1, int a2) {
    //
}           

//SYSCALL > 10
//Devono essere inoltrati al gestore di livello superiore se presente
//(i.e. se è stato specificato da una SYS10),
//altrimenti causano la terminazione del processo.
//Stesso dicasi per le eccezioni di tipo TRAP