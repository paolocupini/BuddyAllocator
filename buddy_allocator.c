#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"

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
            
            printf("\n level %d:\t", ++lev);
            for (int j = 0; j < (1 << tot) - (1 << lev); j++) printf(" "); //stampa spazi
            bit_left = 1 << lev; //ad ogni livello stamo 2^lev
        }
          printf("%d ", BitMap_bit(bit_map, i));
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

  //! To do: add control on sizes -> \\DONE
  if( num_levels>MAX_LEVELS){
    printf("Num di livelli eccessivo\n");
    return 0;
  }

  if (min_bucket_size<3){
    printf("MinBucket non sufficiente\n");
    return 0;
  }

  int num_bits=(1<<(num_levels+1)) -1; 

  alloc->num_levels=num_levels;
  alloc->buffer=buffer;
  alloc->buffer_size=buffer_size;
  alloc->min_bucket_size=min_bucket_size;


   printf("BUDDY INITIALIZING\n");
   printf("\t mem allocator:%d\n",buffer_size);
   printf("\t used for bitmap:%d\n",num_bits);
   printf("\tlevels: %d\n", num_levels);
   printf("\tmin bucket size:%d\n",min_bucket_size);
   printf("\n\n");
   BitMap_init(&alloc->bitmap,num_bits,bit_buf);
   printf("BITMAP ALLOCATED \n");
   bitmap_print(&alloc->bitmap);

   

return 1;

}

//imposto stato genitore
void Set_parent(BitMap *bitmap, int status, int numBit){
    BitMap_setBit(bitmap,status,numBit);
    if (numBit != 0) Set_parent(bitmap,status,parentIdx(numBit));
}
//setto child'status
void Set_child(BitMap *bitmap, int status, int numBit){
    if(numBit<bitmap->num_bits){
        BitMap_setBit(bitmap,status,numBit);
        Set_child(bitmap, status, numBit*2+1); //sx
        Set_child(bitmap, status, numBit*2+2); //dx
    }
}

//! ALLOCAZIONE
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size){
    printf("\t il blocco da allocare ha dimensione: %d\n",size);
    size+=sizeof(int); // 4 bytes indice della bitmap
  
//! case 1: blocco da allocare più grande della memoria disponibile
    if (alloc->buffer_size<size){
      printf("\tMemoria disponibile non sufficiente\n");
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
       printf("\tNon ci sono blocchi disponibili\n");
       return NULL;
    }
//! cambio status dei bit associati a genitore e figio
    Set_parent(&alloc->bitmap,1, freeIdx);
    Set_child(&alloc->bitmap, 1,freeIdx);
    // puntatore al blocco allocato 
    char *addr = alloc->buffer + offsetIdx(freeIdx)* start_size;
    ((int*)addr)[0] = freeIdx;
    bitmap_print(&alloc->bitmap);
    //ritorno puntatore al blocco allocato
    printf("\tallocazione terminata\n");
    return (void *)(addr+sizeof(int));

};






//! FREE
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator *alloc, void *mem){
  //! DA IMPLEMENTARE
    }





