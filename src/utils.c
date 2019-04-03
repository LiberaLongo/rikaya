// utils?

/*
COME MASCHERARE UN BIT IN UN INTERO
Per settare a 1 il bit alla posizione bitPosition menosignificativo
intValue |= 1 << bitPosition;
Per settare a 0 il bit alla posizione bitPosition menosignificativo
intValue &= ~(1 << bitPosition);
*/
int maschera(int variabile, int operazione, int bitPosition ) {
    int result = -1;
    //se operazione = 0 setto il bit a 0
    if( operazione == 0)
        result &= ~(1 << bitPosition);
    //se operazione = 1 setto il bit a 1
    if( operazione == 1)
        result |= 1 << bitPosition;
    return result;
}