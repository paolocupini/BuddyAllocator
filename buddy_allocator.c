#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"
//! debug effects 
/*
printf "Colors:\n"
printf "\033[0;30mHello World! black\033[0m\n"
printf "\033[0;31mHello World! red\033[0m\n"
printf "\033[0;32mHello World! green\033[0m\n"
printf "\033[0;33mHello World! yellow\033[0m\n"
printf "\033[0;34mHello World! blue\033[0m\n"
printf "\033[0;35mHello World! pink\033[0m\n"
printf "\033[0;36mHello World! teal\033[0m\n"
printf "\033[0;37mHello World! white\033[0m\n"

printf "\nBold:\n"
printf "\033[30;1mHello World! grey\033[0m\n"
printf "\033[31;1mHello World! orange\033[0m\n"
printf "\033[32;1mHello World! green\033[0m\n"
printf "\033[33;1mHello World! yellow\033[0m\n"
printf "\033[34;1mHello World! blue\033[0m\n"
printf "\033[35;1mHello World! pink\033[0m\n"
printf "\033[36;1mHello World! teal\033[0m\n"
printf "\033[37;1mHello World! white\033[0m\n"

printf "\nEffects:\n\n"

printf "Black text effects:\n"
printf "\033[30;2mHello World!\033[0m\n"
printf "\033[30;3mHello World!\033[0m\n"
printf "\033[30;4mHello World!\033[0m\n"
printf "\033[30;5mHello World! blink\033[0m\n"
printf "\033[30;6mHello World!\033[0m\n"
printf "\033[30;7mHello World!\033[0m\n"
printf "\033[30;8mHello World!\033[0m\n"

printf "\nRed text effects:\n"

printf "\033[31;2mHello World! faint\033[0m\n"
printf "\033[31;3mHello World! normal\033[0m\n"
printf "\033[31;4mHello World! underline\033[0m\n"
printf "\033[31;5mHello World! blink\033[0m\n"
printf "\033[31;6mHello World! normal, I think\033[0m\n"
printf "\033[31;7mHello World! negative text\033[0m\n"

printf "\nGreen text effects:\n"

printf "\033[32;2mHello World! faint\033[0m\n"
printf "\033[32;3mHello World! normal\033[0m\n"
printf "\033[32;4mHello World! underline\033[0m\n"
printf "\033[32;5mHello World! blink\033[0m\n"
printf "\033[32;6mHello World! normal, I think\033[0m\n"
printf "\033[32;7mHello World! negative text\033[0m\n"

printf "\nYellow text effects:\n"

printf "\033[33;2mHello World! faint\033[0m\n"
printf "\033[33;3mHello World! normal\033[0m\n"
printf "\033[33;4mHello World! underline\033[0m\n"
printf "\033[33;5mHello World! blink\033[0m\n"
printf "\033[33;6mHello World! normal, I think\033[0m\n"
printf "\033[33;7mHello World! negative text\033[0m\n"

printf "\nBlue text effects:\n"

printf "\033[34;2mHello World! faint\033[0m\n"
printf "\033[34;3mHello World! normal\033[0m\n"
printf "\033[34;4mHello World! underline\033[0m\n"
printf "\033[34;5mHello World! blink\033[0m\n"
printf "\033[34;6mHello World! normal, I think\033[0m\n"
printf "\033[34;7mHello World! negative text\033[0m\n"

printf "\nPink text effects:\n"

printf "\033[35;2mHello World! faint\033[0m\n"
printf "\033[35;3mHello World! normal\033[0m\n"
printf "\033[35;4mHello World! underline\033[0m\n"
printf "\033[35;5mHello World! blink\033[0m\n"
printf "\033[35;6mHello World! normal, I think\033[0m\n"
printf "\033[35;7mHello World! negative text\033[0m\n"

printf "\nTeal text effects:\n"

printf "\033[36;2mHello World! faint\033[0m\n"
printf "\033[36;3mHello World! normal\033[0m\n"
printf "\033[36;4mHello World! underline\033[0m\n"
printf "\033[36;5mHello World! blink\033[0m\n"
printf "\033[36;6mHello World! normal, I think\033[0m\n"
printf "\033[36;7mHello World! negative text\033[0m\n"

printf "\nWhite text effects:\n"

printf "\033[37;2mHello World! faint\033[0m\n"
printf "\033[37;3mHello World! normal\033[0m\n"
printf "\033[37;4mHello World! underline\033[0m\n"
printf "\033[37;5mHello World! blink\033[0m\n"
printf "\033[37;6mHello World! normal, I think\033[0m\n"
printf "\033[37;7mHello World! negative text\033[0m\n"





*/
// these are trivial helpers to support you in case you want
// to do a bitmap implementation
//! copy and paste from prof's code

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


void bitmap_print(BitMap *bit_map){
    int tot = levelIdx(bit_map->num_bits) - 1;  //numero di livelli totale
    int bit_left = 0; //bit rimasti da stampare per ogni lev
    int lev = -1; 
    for (int i = 0; i < bit_map->num_bits; i++){  
        if (bit_left == 0){ //lev terminato
            if(lev==tot) break; //finito 
            
            printf("\n\033[0;36m%d\033[0m\t", ++lev);
            for (int j = 0; j < (1 << tot) - (1 << lev); j++) printf(" "); //stampa spazi
            bit_left = 1 << lev; //ad ogni livello stampo 2^lev
        }
          if (BitMap_bit(bit_map, i)==1){
            printf("\033[0;32m%d\033[0m ",BitMap_bit(bit_map,i));

          }
          else{
            printf("\033[0;34m%d\033[0m ",BitMap_bit(bit_map,i));

          }
          bit_left--;  //1 bit in meno da stampare
    }
    printf("\n");
};

int BuddyAllocator_init(BuddyAllocator* alloc,
                        int num_levels,
                        char *buffer, // buffer per allocazione
                        int buffer_size, 
                        char *bit_buf,  //buffer per bitmab
                        int bit_buf_size,
                        int min_bucket_size
){
  printf("\033[0;33m...INITIALIZING BUDDY...\033[0m\n");
  sleep(2);


  if( num_levels>MAX_LEVELS){
    printf("Num di livelli eccessivo\n");
    return 0;
  }

  if (min_bucket_size<4){
    printf("MinBucket non sufficiente\n");
    return 0;
  }


  int num_bits=(1<<(num_levels+1)) -1; 

  alloc->num_levels=num_levels;
  alloc->buffer=buffer;
  alloc->buffer_size=buffer_size;
  alloc->min_bucket_size=min_bucket_size;

  // STAMPIAMO INFO DELL'ALBERO BINARIO CORRISPONDENTE ALLA BITMAP
   printf("\tTREE STORING:%d\n",buffer_size);
   printf("\tTREE NODES:%d\n",num_bits);
   printf("\tLEVELS: %d\n", num_levels);
   printf("\tMIN BUCKET:%d\n",min_bucket_size);
   printf("\033[32;1m...BUDDY INITIALIZED...\033[0m");
   printf("\n\n");
   printf("\033[0;33m....ALLOCATING BITMAP...\033[0m\n");
   sleep(2);
   BitMap_init(&alloc->bitmap,num_bits,bit_buf);
   printf("\033[32;1m...BITMAP ALLOCATED...\033[0m");
   bitmap_print(&alloc->bitmap);

   

return 1;

}

//imposto stato genitore
void Set_parent(BitMap *bitmap, int status, int numBit){
    BitMap_setBit(bitmap,numBit,status);
    if (numBit != 0) Set_parent(bitmap,status,parentIdx(numBit));
}
//setto child'status
void Set_child(BitMap *bitmap, int status, int numBit){
    if(numBit<bitmap->num_bits){
        BitMap_setBit(bitmap,numBit,status);
        Set_child(bitmap, status, numBit*2+1); //sx
        Set_child(bitmap, status, numBit*2+2); //dx
    }
}

//! ALLOCAZIONE
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size){
    
    size+=sizeof(int); // 4 bytes indice della bitmap
    sleep(3);
    printf("\033[0;33m\nALLOCAZIONE BLOCCO DIM:%d\033[0m\n",size);


//! case 1: blocco da allocare più grande della memoria disponibile
    if (alloc->buffer_size<size){
    printf("\n\033[0;31m*************************ERROR: MEMORIA NON SUFFICIENTE************************\033[0m\n");

      return NULL;
    }
//! case 2: memoria disponibile, cerco livello
    int block_lev = alloc->num_levels;
    int start_size = alloc->min_bucket_size;
    for (int i=0; i< alloc->num_levels;i++){
        if(start_size<size) {
          start_size*=2;
          block_lev--; //level up
        }
        else break;
    }
//! case 3: cerco indice libero
    int freeIdx = -1;
    int i;
    for (i=firstIdx(block_lev); i<firstIdx(block_lev+1);i++){
        if(!BitMap_bit(&alloc->bitmap,i)){ //indice i libero
            freeIdx = i;
            break;
        }
    }
//! case 4: non abbiamo blocchi disponibili
    if (freeIdx ==-1){
       printf("\033[0;31m*******************ERROR: NON CI SONO BLOCCHI DISPONIBILI*******************\033[0m\n");
       return NULL;
    }
    printf("\033[0;36mLVELLO:%d\nINDICE:%d\033[0m\n",block_lev,freeIdx);

//! cambio status dei bit associati a genitore e figio
    Set_parent(&alloc->bitmap,1, freeIdx);
    Set_child(&alloc->bitmap, 1,freeIdx);
    // puntatore al blocco allocato 
    char *addr = alloc->buffer + offsetIdx(freeIdx)* start_size;
    ((int*)addr)[0] = freeIdx;
    sleep(2);
    printf("\n\033[0;32mBITMAP DOPO ALLOCAZIONE\033[0m\n");
    
    bitmap_print(&alloc->bitmap);
    //ritorno puntatore al blocco allocato
    printf("\n\033[0;32m**********************BLOCCO ALLOCATO CON SUCCESSO**********************\033[0m\n");
    sleep(2);
    return (void *)(addr+sizeof(int));

};

//! FREE
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator *alloc, void *mem){
  int *p = (int *)mem;
  int i_0= p[-1];
  

  //rimettiamo a 0 il bit nella bitmap 
  Set_child(&alloc->bitmap, 0,i_0);
  Bitmap_merge(&alloc->bitmap,i_0); //riunisco i buddy ricorsivamente

  printf("\n\033[0;32mBITMAP DOPO LA FREE\033[0m\n");
  bitmap_print(&alloc->bitmap);
  printf("\n\033[0;32m***********************BLOCCO:%d LIBERATO CON SUCCESSO**********************\033[0m\n",i_0);
sleep(2);
}
//! MERGE
// funzione ricorsviva di merging fino alla radice
void Bitmap_merge(BitMap *bitmap, int idx){
  //siamo alla radice
  if (idx == 0) return;
  
  //merge di blocco idx e di buddy(idx)
  int buddy_idx = buddyIdx(idx);
  if (!BitMap_bit(bitmap, buddy_idx)){
    int parent_idx = parentIdx(idx);
    BitMap_setBit(bitmap, parent_idx, 0);
    Bitmap_merge(bitmap, parent_idx);
  }
 

}




