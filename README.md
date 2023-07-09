# SO-project

Progetto Sistemi Operativi 2022/23, Prof. Giorgio Grisetti

[TO-DO] Line-up:

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
spiegare funzioni di malloc, free e merge.





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

