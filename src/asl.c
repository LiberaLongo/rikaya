#include "../header/const.h"
#include "../header/listx.h"
#include "../header/pcb.h"
#include "../header/asl.h"
#include "../header/types_rikaya.h"

semd_t semd_table[MAXPROC];

LIST_HEAD(semdFree);
struct list_head *semdFree_h = &semdFree;

LIST_HEAD(semd);
struct list_head *semd_h = &semd;

/* funzione ausiliaria che controlla se il semaforo ha finito i processi
se la sua lista è vuota lo rimette nei semafori liberi*/
    //Se la coda dei processi bloccati per il semaforo diventa vuota
void    deallocSemd(struct semd_t *semd)
{
    if (list_empty(&semd->s_procQ))
    {
        //lo elimino dalla lista degli attivi
        list_del(&semd->s_next);
        //lo metto nella lista dei semafori liberi
        list_add_tail(&semd->s_next, semdFree_h);
    }
}

/*14. semd_t* getSemd(int *key)
DESCRIZIONE:
Restituisce il puntatore al SEMD nella ASL la cui chiave è pari a key.
Se non esiste un elemento nella ASL con chiave eguale a key, viene restituito NULL.
*/
struct semd_t *getSemd(int *key)
{
    if (!(key == NULL || list_empty(semd_h)))
    {
        struct list_head *iter;
        //per ogni elemento della lista viene confrontata la key, si ritorna il semaforo se viene trovata
        list_for_each(iter, semd_h)
        {
            struct semd_t *temp;
            temp = container_of(iter, semd_t, s_next);
            if (temp->s_key == key)
                return temp;
        }
    }
    //se key NULL, la lista è vuota, o la chiave non viene trovata si restituisce NULL
    return NULL;
}

/* 

15. int insertBlocked(int *key, pc_t *pb)
DESCRIZIONE: Viene inserito il PCB puntato da p nella coda 
dei processi bloccati associata al SEMD con chiave key.
Se il semaforo corrispondente non è presente nella ASL,
alloca un nuovo SEMD dalla lista di quelli liberi (semdFree)
e lo inserisce nella ASL, settando i campi in maniera
opportuna (i.e. key e s_procQ).
Se non è possibile allocare un nuovo SEMD perché la lista di quelli liberi è vuota,
restituisce TRUE. In tutti gli altri casi, restituisce FALSE. 
*/
int insertBlocked(int *key, struct pcb_t *p)
{
    if (!(p == NULL || key == NULL))
    {
        struct semd_t *temp = getSemd(key);
        if (temp != NULL)
        {
            //nella lista s_procQ del semd trovato si inserisce il pcb
            p->p_semkey = key;
            list_add_tail(&p->p_next, &temp->s_procQ);
            return FALSE;
        }
        //se non viene trovato il semaforo, si alloca un semaforo libero
        if (list_empty(semdFree_h))
            return TRUE;
        struct list_head *freeLHSem = list_next(semdFree_h);
        struct semd_t *freeSem = container_of(freeLHSem, semd_t, s_next);

        //aggiornamento campi key
        p->p_semkey = key;
        freeSem->s_key = key;

        //rimozione del semaforo dalla lista dei semdFree e aggiunta agli ASl
        list_del(freeLHSem);
        list_add_tail(freeLHSem, semd_h);

        //inserimemto del pcb nella coda dei processi del semaforo appena aggiunto
        INIT_LIST_HEAD(&freeSem->s_procQ);
        list_add_tail(&p->p_next, &freeSem->s_procQ);
    }
    return FALSE;
}

/*
16. pcb_t* removeBlocked(int *key)
DESCRIZIONE: Ritorna il primo PCB dalla
coda dei processi bloccati (s_ProcQ)
associata al SEMD della ASL con chiave key.
Se tale descrittore non esiste nella ASL,
restituisce NULL. Altrimenti, restituisce
l’elemento rimosso. Se la coda dei processi
bloccati per il semaforo diventa vuota,
rimuove il descrittore corrispondente dalla
ASL e lo inserisce nella coda dei descrittori
liberi (semdFree).
*/
struct pcb_t *removeBlocked(int *key)
{
    struct semd_t *temp = getSemd(key);
    if (temp == NULL)
        return NULL;

    struct pcb_t *process = removeProcQ(&temp->s_procQ);

    //Se la coda dei processi bloccati per il semaforo diventa vuota
    deallocSemd(temp);
    return process;
}
/*
17. pcb_t* outBlocked(pcb_t *p)
DESCRIZIONE: Rimuove il PCB puntato da p dalla
coda del semaforo su cui è bloccato (indicato da p-
>p_semkey). Se il PCB non compare in tale coda,
allora restituisce NULL (condizione di errore).
Altrimenti, restituisce p.
*/
/*- Le funzioni outBlocked e outChildBlocked
non rimettono il semaforo nella lista dei semafori liberi
se non ci sono piu' processi bloccati su di lui (come fa removeBlocked)*/
struct pcb_t *outBlocked(struct pcb_t *p)
{
    if (p != NULL)
    {

        struct semd_t *temp = getSemd(p->p_semkey);
        //rimozione processo
        if (temp != NULL)
        {
            //elimina il processo dalla lista del suo semaforo
            struct pcb_t *ret = outProcQ(&temp->s_procQ, p);
            deallocSemd(temp);
            return ret;
        }
    }
    return NULL;
}
/*
18. pcb_t* headBlocked(int *key)
DESCRIZIONE: Restituisce (senza rimuovere) il
puntatore al PCB che si trova in testa alla coda dei
processi associata al SEMD con chiave key. Ritorna
NULL se il SEMD non compare nella ASL oppure
se compare ma la sua coda dei processi è vuota.
*/
struct pcb_t *headBlocked(int *key)
{
    if (key != NULL)
    {
        struct semd_t *temp = getSemd(key);
        if (temp != NULL)
        {
            return headProcQ(&temp->s_procQ);
        }
    }
    return NULL;
}

/*
19. void outChildBlocked(pcb_t *p)
DESCRIZIONE: Rimuove il PCB puntato da p dalla
coda del semaforo su cui è bloccato (indicato da p-
>p_semkey). Inoltre, elimina tutti i processi
dell’albero radicato in p (ossia tutti i processi che
hanno come avo p) dalle eventuali code dei
semafori su cui sono bloccati.
*/

void aux(struct pcb_t *p)
{
/*
funzione ausiliaria che elimina dalle code dei processi dei semafori
il processo passato, i suoi fratelli ed i suoi discendenti
*/
    outBlocked(p);

    if (p == NULL)
        return;

    if (!(list_is_last(&p->p_sib, &(p->p_parent)->p_child)))
        //ricorsione sui fratelli
        aux(container_of(list_next(&p->p_sib), pcb_t, p_sib));

    if (emptyChild(p))
        return;
    //ricorsione sui figli
    aux(container_of(list_next(&p->p_child), pcb_t, p_sib));
}

void outChildBlocked(struct pcb_t *p)
{
    //si rimuove p dalla coda dei processi
    outBlocked(p);
    aux(container_of(list_next(&p->p_child), pcb_t, p_sib));
}

/*
20. void initASL()
DESCRIZIONE: Inizializza la lista dei semdFree in
modo da contenere tutti gli elementi della
semdTable. Questo metodo viene invocato una
volta sola durante l’inizializzazione della struttura
dati.
*/
void initASL(void)
{
    //inizializzazione della lista dei semdFree
    INIT_LIST_HEAD(semdFree_h);
    for (int i = 0; i < MAXPROC; i++)
    {
        INIT_LIST_HEAD(&semd_table[i].s_procQ);
        list_add_tail(&semd_table[i].s_next, semdFree_h);
    }
}
