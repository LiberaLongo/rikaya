RIKAYA
progetto di sistemi operativi basato su uMPS2.

COMPILAZIONE ED ESECUZIONE SU UMPS
scaricare uMPS2.
aprire il terminale (Ctrl + Alt + T)
per compilare digitare il comando
"make"
per pulire prima di ricompilare
"make clean"
succesivamente
"umps2-elf2umps -k kernel"
e lanciare umps con
"umps"
e configurare i campi con i kernel.core.umps e kernel.stab.umps

SPECIFICHE

p1test_rikaya_v0.c: funzioni test

pcb.c: implementazione degli alberi e delle code dei processi
        e la lista dei processi liberi o non utilizzati.
asl.c: implementazione di una Lista di Semafori Attivi (ASL Active Semaphore List)
        per gestire i processi bloccati.

const.h: definizione delle costanti usate.
listx.h: un API per liste circolari bidirezionali, con sentinella,
        utilizzata per ASL e PCB.
types_rikaya.h: file contenente le strutture dei PCB e dei descrittori dei semafori.
pcb.h: dichiarazione delle funzioni di PCB.
asl.h: dichiarazione delle funzioni di ASL.

SCELTE PROGETTUALI
Le liste dei PCB, PCBFree, degli ASL, e dei SemdFree sono state implementate tramite liste circolari, bidirezionali, con sentinalla. La sentinella è stata implementata di tipo list_head.
Quando possibile sono state utilizzate funzioni inline e macro presenti in listx.h e sono state riutilizzate le funzioni create precedentemente.
Nella funzione outChildBlocked() è stata implementata una funzione ricorsiva ausiliaria.
Le funzione sono state implementate e testate collettivamente da tutti i componenti del gruppo.

In fase di sviluppo è stato fatto riferimento, come guida concettuale alle funzioni, il codice di 
https://github.com/Maldus512

PROGETTO GITHUB DI UMPS2 DI TJONJIC
git clone https://github.com/tjonjic/umps