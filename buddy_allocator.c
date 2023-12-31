#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"
#include <unistd.h>


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
    int tot_allocated=0;
    for (int i = 0; i < bit_map->num_bits; i++){  
        if (bit_left == 0){ //lev terminato
            if(lev==tot) break; //finito 
            
            printf("\n\033[0;36m%d\033[0m\t", ++lev);
            for (int j = 0; j < (1 << tot) - (1 << lev); j++) printf(" "); //stampa spazi
            bit_left = 1 << lev; //ad ogni livello stampo 2^lev
        }
          if (BitMap_bit(bit_map, i)==1){
           printf("\033[0;32m%d\033[0m ",BitMap_bit(bit_map,i));
            tot_allocated++;

          }
          else{
          printf("\033[0;34m%d\033[0m ",BitMap_bit(bit_map,i));

          }
          bit_left--;  //1 bit in meno da stampare
    }
     printf("\n");
    printf("\n\033[0;35mPERCENTUALE DI BITMAP OCCUPATA:%.1f%%\033[0m\n",(float)tot_allocated/bit_map->num_bits *100);

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

  if (min_bucket_size<1){
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
  if (mem==NULL){
    printf("\n\033[0;31m**************************ERROR: BLOCCO NON ALLOCATO***************************\033[0m\n");
    return;
  }
  int *p = (int *)mem;
  int i_0= p[-1];

  // evito la free di blocchi già liberati
  if (BitMap_bit(&alloc->bitmap, i_0)==0){
    printf("\n\033[0;31m**************************ERROR: BLOCCO GIA LIBERATO***************************\033[0m\n");
    return;
  }
  
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




