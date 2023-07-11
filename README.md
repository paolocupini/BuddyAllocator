# SO-project

Progetto Sistemi Operativi 2022/23, Prof. Giorgio Grisetti


## 1.Spiegazione scopo del progetto
Il progetto ha come obiettivo l'implementazione di un allocator di memoria che non faccia uso della malloc. Nello specifico l'allocazione viene gestita in due modi; attraverso un BuddyAllocator per piccole allocazioni e attraverso MMAP. Il BuddyAllocator divide la memoria ricorsivamente, ad ogni ricorsione un blocco genitore viene diviso in due "buddys" figli, finchè non si arriva al punto che un ulteriore ricorsione renderebbe il blocco più piccolo della memoria richiesta. Possiamo vedere il risultato della divisione ricorsiva come un albero binario, possiamo quindi sfruttare le proprietà di relazione dei nodi:
``` c
//level of node idx
int levelIdx(size_t idx){
  return (int)floor(log2(idx+1));
};
//index of the buddy with idx 
int buddyIdx(int idx){ 
  if (idx==0) return 0;
  else if (idx &0x1) return idx+1;
  return idx-1;
}

int parentIdx(int idx){
    return (idx-1)/2;
}
// ritorna indice primo nodo sul livello i , corrisponde a 2^i 
int firstIdx(int lev){
  return (1 << lev)-1; //2^level -1
}

// ritorna distanza dal primo elemento sul livello dato
int offsetIdx(int idx){
    return (idx-(firstIdx(levelIdx(idx))));
}

```


## 2.Scelte implementative e spiegazione funzioni principali

Il progetto si sviluppa a partire dal codice visto a lezione con la differenza di salvare lo stato dell'albero in una bitmap: ogni blocco è associato ad un bit 1/0 a seconda se sia occupato o no.

La struct `BitMap` conserva un buffer di memoria con la relativa dimensione e il numero di bit che può contenere corrispondenti al numero di nodi dell'albero. 

Abbiamo già visto a lezione le funzioni implementate nel file 'bit_map.c', con lo scopo inizialiazzare la bitmap e interrogare/settare il contenuto dei bit.

La struct `BuddyAllocator` salva le informazioni per la gestione della bitmap, come il numero di livelli dell'albero, la minima dimensione di un blocco e inoltre le informazioni riguardanti la quantità di memoria che può essere gestita.

1. La funzione `BuddyAllocator_malloc`ha il seguente comportamento: controlla se la memoria disponibile è sufficiente per soddisfare la richiesta,  ricerca il livello in cui allocare la memoria partendo dal livello più in basso, itera sui blocchi del livello trovato alla ricerca del blocco libero. Una volta trovato il blocco modifica lo stato del bit del blocco selezionato e dei nodi genitori fino alla radice attraverso le funzioni 'Set_parent' e 'Set_child'.  

2. La funzione `BuddyAllocator_free`: non fa altro che controllare che l'indirizzo passato corrisponda ad un blocco allocato in precedenza e nel caso cambia il bit corrispondente nella Bitmap. 

3. La funzione `Bitmap_merge` ricompatta i blocchi, riunendo ricorsivamente i due buddys con il nodo genitore fino alla radice.

## 3.Come eseguire

```console

paolocupini@def:~$ make
paolocupini@def:~$ ./buddy_allocator_test "x"


```
Dove la variabile "x" può assumere i seguenti valori: 
1. x=-1 -> esegue tutti i test relativi ad allocazioni con BuddyAllocator
2. x=-2 -> esegue tutti i test relativi ad allocazioni con mmap
3. x>0 -> gestisce l'allocazione in due modi: tramite BuddyAllocator se x<1024, altrimenti con mmap.
## 4.Spiegazione test 

Per visualizzare meglio le allocazioni e deallocazioni viene testato un buddyallocator con 5 livelli, 

