## DOCUMENTAZIONE

### SEZIONE FASE 1

#### SPECIFICHE

p1test_rikaya_v0.c: funzioni test

i source:
- pcb.c : implementazione degli alberi e delle code dei processi
        e la lista dei processi liberi o non utilizzati.
- asl.c : implementazione di una Lista di Semafori Attivi (ASL Active Semaphore List)
        per gestire i processi bloccati.

gli headers:
- const.h : definizione delle costanti usate.
- listx.h : un API per liste circolari bidirezionali, con sentinella,
        utilizzata per ASL e PCB.
- types_rikaya.h : descrittori dei semafori.
- pcb.h : dichiarazione delle funzioni di PCB.
- asl.h : dichiara file contenente le strutture dei PCB e dei desrazione delle funzioni di ASL.

#### SCELTE PROGETTUALI

Le liste dei PCB, PCBFree, degli ASL, e dei SemdFree sono state implementate tramite liste circolari, bidirezionali, con sentinella. La sentinella è stata implementata di tipo list_head.
Quando possibile sono state utilizzate funzioni inline e macro presenti in listx.h e sono state riutilizzate le funzioni create precedentemente.
Nella funzione outChildBlocked() è stata implementata una funzione ricorsiva ausiliaria.
Le funzione sono state implementate e testate collettivamente da tutti i componenti del gruppo.

### SEZIONE FASE 1.5

#### SPECIFICHE

Oltre ai file di fase 1 vi sono i seguenti file:

i source:
- p1.5test_rikaya_v0.c: contiene le funzioni di test,
        e la funzione per la stampa del diagramma di Gantt.
- init.c : file che gestisce l'inizializzazione delle newArea, setta gli stati dei processi e li inserisce nella readyQueue.
- handler.c : contiene le funzioni che differenziano e gestiscono i diversi tipi di eccezione.
- interrupt.c : contiene le funzioni per gestire gli interrupt.
- syscall.c : contiene le funzioni per gestire le system call.
- scheduler.c : contiene le funzioni dello scheduler, della gestione del timer.
- utils.c : contiene le funzioni per mascherare i bit desiderati, per copiare lo stato del processo,
        e isolare i bit del cause(causeIp, causeExcCode).
- main.c : si preoccupa dell'inizializzazione e del lancio in esecuzione del primo processo.

gli headers:
- init.h
- handler.h
- interrupt.h
- syscall.h
- scheduler.h
- utils.h : contiene le definizioni di costanti richiamate in tutto il codice
- p15test.h : contiene alcune definizioni presenti in p1.5test_rikaya_v0.c usate nel resto del codice
        in modo da tenere p1.5test_rikaya_v0.c solo come test

#### SCELTE PROGETTUALI

Il nucleo gestisce le seguenti funzionalita:
- inizializzazione del sistema.
- scheduling dei processi.
- gestione delle system call.
- gestione degli interrupt.

**inizializzazione del sistema**:

- vengono inizializzate le newArea
        (settando PC, SP, IM, TE ).
- vengono settati gli stati dei processi
        (settando PC, SP, KUp, VMp, IEp, IM, priority e original priority ).
- vengono inseriti i PCB relativi ai processi nella ready Queue.

**scheduling dei processi**:

lo scheduler esegue il context switch dei processi con time_slice di 3ms
con un meccanismo di aging per evitare starvation.
il nostro scheduler prevede una funzione timerSetting()
che calcola il valore da attribuire al timer.

lo scheduler per effettuare il context switch con aging:
- preleva il processo in testa dalla ready queue (che diventa il processo corrente).
- aggiorna la priorità del processo alla priorità originale.
- incrementa le priorità dei processi rimanenti nella ready Queue con la funzione aging().
- reinserisce il processo rimosso precedentemente nella ready Queue.
- solo successivamente viene attivato il timer.
- e caricato in esecuzione il processo corrente.

**gestione delle system call**:

con il campo CauseExcCode verifichiamo se si tratta di un breackpoint o di una system call
e successivamente con il valore del campo a0 identifichiamo la system call
(in questa fase è stata gestita solo la systemcall 3,
altrimenti il sistema viene messo in panic)
appurato che sia una system call 3 il processo corrente viene rimosso dalla ready Queue
e il controllo viene passato allo scheduler per lanciare il processo successivo.

**gestione degli interrupt**:

con il campo CauseIP verichiamo la linea da cui proviene l'interrupt
(in questa fase è stato gestito solo l'interrupt del processor local timer,
altrimenti il sistema viene messo in panic)
appurato che sia l'interrupt del processor local timer
il gestore dell'interrupt passa il controllo allo scheduler.

in **utils.c** sono state implementate delle funzioni ausiliarie
maskBit() per mascherare il bit desiderato di un binario
getCauseField() per isolare i bit di CauseIP e CauseExcCode prima dell'identificazione.