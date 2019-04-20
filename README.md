## RIKAYA
progetto di sistemi operativi basato su uMPS2.

### SCARICAMENTO, COMPILAZIONE ED ESECUZIONE SU uMPS2

scaricare uMPS2.

1. aprire il terminale
(Ctrl + Alt + T)

2. clonare la cartella del progetto:
da comando:
`git clone http://www.github.com/LiberaLongo/rikaya.git`
oppure da github tramite zip.

3. spostarsi dentro la cartella rikaya nella sottocartella src
`cd rikaya/src`

4. per compilare digitare il comando
`make`
(per pulire tutto (non necessario per la ricompilazione))
`make clean`

5. lanciare umps con
`umps2` oppure con `umps2 &` (quest'ultimo serve per mantenere umps2 aperto e poter digitare altri comandi da terminale)

6. configurare i campi con i kernel.core.umps e kernel.stab.umps dentro l'IDE di uMPS2

### specifiche e scelte progettuali

In DOCUMENTAZIONE.md.

### ALTRE INFORMAZIONI

In fase di sviluppo è stato fatto riferimento, come guida concettuale alle funzioni, il codice di 
<https://github.com/Maldus512>

progetto github di umps2 di Tjonjic
git clone <https://github.com/tjonjic/umps>
