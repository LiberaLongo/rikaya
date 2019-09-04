#include "../header/const.h"
#include "../header/listx.h"
#include "../header/pcb.h"
#include "../header/types_rikaya.h"

/*
• pcbFree: lista dei PCB che sono liberi o
inutilizzati.
• pcbfree_h: elemento sentinella della lista
pcbFree.
• pcbFree_table
*/
LIST_HEAD(pcbFree);
struct list_head *pcbFree_h = &pcbFree;
struct pcb_t pcbFree_table[MAXPROC];

/*
1. void initPcbs()
DESCRIZIONE: Inizializza la pcbFree in modo
da contenere tutti gli elementi della
pcbFree_table. Questo metodo deve essere
chiamato una volta sola in fase di
inizializzazione della struttura dati.
*/
void initPcbs(void)
{
    //Inizializzazione della pcbFree e inserimento in coda dei pcb presenti nella pcbFree_table
    INIT_LIST_HEAD(pcbFree_h);

    for (int i = 0; i < MAXPROC; i++)
    {
        list_add_tail(&pcbFree_table[i].p_next, pcbFree_h);
    }
}

/*
2. void freePcb(pcb_t *p)
DESCRIZIONE: Inserisce il PCB puntato da
p nella lista dei PCB liberi (pcbFree)
*/
void freePcb(struct pcb_t *p)
{
    if (p != NULL)
        list_add_tail(&p->p_next, pcbFree_h);
}

/*
3. struct pcb_t *allocPcb(void)
DESCRIZIONE: Restituisce NULL se la pcbFree è
vuota. Altrimenti rimuove un
elemento dalla pcbFree, inizializza tutti i 
campi (NULL/0) e restituisce l’elemento
rimosso.
*/
struct pcb_t *allocPcb(void)
{
    if (list_empty(pcbFree_h))
        return NULL;
    else
    {
        //puntatore temporaneo al primo pcb della lista
        struct pcb_t *temp;
        temp = container_of(list_next(pcbFree_h), pcb_t, p_next);

        list_del(list_next(pcbFree_h));

        //puntatori di pcb_t
        temp->p_parent = NULL;
        temp->p_semkey = NULL;

        //interi di pcb_t
        temp->priority = 0;
        //specifiche fase 1.5
        temp->original_priority = 0;

        //list_heads
        temp->p_next.next = NULL;
        temp->p_next.prev = NULL;
        temp->p_child.next = NULL;
        temp->p_child.prev = NULL;
        temp->p_sib.next = NULL;
        temp->p_sib.prev = NULL;

        //campi time
        temp->clock_wall = 0;
        temp->kernel_time = 0;
        temp->user_time = 0;

        //campo tutor
        temp->tutorFlag = FALSE;
        
        //stato
        temp->p_s.entry_hi = 0;
        temp->p_s.cause = 0;
        temp->p_s.status = 0;
        temp->p_s.pc_epc = 0;
        temp->p_s.hi = 0;
        temp->p_s.lo = 0;

        //per gestire syscall SpecPassUp
        for (int i = 0; i < 3; i++)
        {
            temp->oldAreaHandler[i] = NULL; 
            temp->newAreaHandler[i] = NULL;
        }

        //array gpr
        for (int i = 0; i < STATE_GPR_LEN; i++)
        {
            temp->p_s.gpr[i] = 0;
        }
        return temp;
    }
}

/*
4. void mkEmptyProcQ(struct list_head *head)
DESCRIZIONE: Inizializza la lista dei PCB,
inizializzando l’elemento sentinella.
*/
void mkEmptyProcQ(struct list_head *head)
{
    if (head == NULL)
    {
        LIST_HEAD(pcbList);
        head = &pcbList;
    }
    INIT_LIST_HEAD(head);
}

/*
5. int emptyProcQ(struct list_head* head)
DESCRIZIONE: Restituisce TRUE se la
lista puntata da head è vuota, FALSE
altrimenti
*/
int emptyProcQ(struct list_head *head)
{

    return list_empty(head);
}

/*
6. void insertProcQ(struct list_head *head, struct pcb_t *p)
DESCRIZIONE: Inserisce l’elemento puntato da p
nella coda dei processi puntata da head.
L’inserimento deve avvenire tenendo conto della
priorita’ di ciascun pcb (campo p->priority). La coda
dei processi deve essere ordinata in base alla priorita’
dei PCB, in ordine decrescente (i.e. l’elemento di
testa è l’elemento con la priorita’ più alta).
*/
void insertProcQ(struct list_head *head, struct pcb_t *p)
{
    if (!(head == NULL || p == NULL))
    {
        struct list_head *iter;

        if (list_empty(head))
        {
            //se vuota inserimento in coda
            list_add_tail(&p->p_next, head);
        }
        else
        {
            list_for_each(iter, head)
            {
                //confronto priorità per ogni elemento della lista
                struct pcb_t *temp;
                temp = container_of(iter, pcb_t, p_next);
                //inserimento in coda
                if (p->priority > temp->priority)
                {
                    __list_add(&p->p_next, list_prev(iter), iter);
                    break;
                }
                //caso in cui p abbia la priorità più bassa
                else if (list_is_last(iter, head))
                {
                    list_add_tail(&p->p_next, head);
                    break;
                }
            }
        }
    }
}

/*
7.struct pcb_t *headProcQ(struct list_head *head)
DESCRIZIONE: Restituisce l’elemento di testa della
coda dei processi da head, SENZA RIMUOVERLO.
Ritorna NULL se la coda non ha elementi.
*/
struct pcb_t *headProcQ(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return NULL;
    else
    {
        //puntatore al primo pcb
        return container_of(list_next(head), pcb_t, p_next);
    }
}

/*
8. struct pcb_t * removeProcQ(struct list_head *head)
DESCRIZIONE: Rimuove il primo elemento dalla
coda dei processi puntata da head. Ritorna NULL
se la coda è vuota. Altrimenti ritorna il puntatore
all’elemento rimosso dalla lista.
*/
struct pcb_t *removeProcQ(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return NULL;
    else
    {
        struct pcb_t *temp = container_of(list_next(head), pcb_t, p_next);
        //rimozione del primo elemento della lista
        list_del(list_next(head));
        return temp;
    }
}

/*
9. struct pcb_t *outProcQ(struct list_head *head, struct pcb_t *p)
DESCRIZIONE: Rimuove il PCB puntato da p dalla
coda dei processi puntata da head. Se p non è
presente nella coda, restituisce NULL. (NOTA: p
può trovarsi in una posizione arbitraria della coda).
*/
struct pcb_t *outProcQ(struct list_head *head, struct pcb_t *p)
{
    if (head == NULL || p == NULL || list_empty(head))
        return NULL;
    else
    {
        struct list_head *iter;
        //per ogni elemento della lista si confronta p con l'elemento corrente
        list_for_each(iter, head)
        {
            struct pcb_t *temp = container_of(iter, pcb_t, p_next);
            if (temp == p)
            {
                list_del(&(temp->p_next));
                return p;
            }
        }
        //nel caso la lista non contenga p
        return NULL;
    }
}

/*
10. int emptyChild(struct pcb_t *p)
DESCRIZIONE: restituisce TRUE se il PCB
puntato da p non ha figli, restituisce FALSE
altrimenti.
*/
int emptyChild(struct pcb_t *p)
{
    if (p != NULL)
    {
        //se la lista non è inizializzata p non ha figli
        if (p->p_child.next == NULL || p->p_child.prev == NULL)
        {
            return TRUE;
        }
        return list_empty(&p->p_child);
    }
    //se p non esiste ha tutte le proprietà
    return TRUE;
}

/*
11. void insertChild(pcb_t *prnt,pcb_t *p)
DESCRIZIONE: Inserisce il PCB puntato da p
come figlio del PCB puntato da prnt (parent).
*/
void insertChild(struct pcb_t *prnt, struct pcb_t *p)
{
    if (!(prnt == NULL || p == NULL))
    {
        //se la lista non esiste viene inizializzata
        if (list_next(&prnt->p_child) == NULL || list_prev(&prnt->p_child) == NULL) 
        {
            INIT_LIST_HEAD(&prnt->p_child);
        }
        //aggiornamento campo parent e inserimento nella lista dei figli
        p->p_parent = prnt;
        list_add_tail(&p->p_sib, &prnt->p_child);
    }
}

/*
12. pcb_t* removeChild(pcb_t *p)
DESCRIZIONE. Rimuove il primo figlio del PCB
puntato da p. Se p non ha figli, restituisce NULL.
*/
struct pcb_t *removeChild(struct pcb_t *p)
{
    if (p != NULL)
    {
        //se p non ha figli
        if (emptyChild(p))
            return NULL;

        //rimozione primo figlio
        struct pcb_t *temp = container_of(list_next(&p->p_child), pcb_t, p_sib); // p_sib sostituito a p_child
        list_del(list_next(&p->p_child));
        temp->p_parent = NULL;
        return temp;
    }
    return NULL;
}

/*
13. pcb_t *outChild(pcb_t* p)
DESCRIZIONE: Rimuove il PCB puntato
da p dalla lista dei figli del padre. Se il
PCB puntato da p non ha un padre,
restituisce NULL. Altrimenti restituisce
l’elemento rimosso (cioè p). A
differenza della removeChild, p può
trovarsi in una posizione arbitraria
(ossia non è necessariamente il primo
figlio del padre).
*/
struct pcb_t *outChild(struct pcb_t *p)
{
    if (p == NULL || p->p_parent == NULL)
        return NULL;
    //rimozione figlio dalla lista
    p->p_parent = NULL;
    list_del(&p->p_sib);
    return p;
}
