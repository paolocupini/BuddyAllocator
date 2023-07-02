#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"

// these are trivial helpers to support you in case you want
// to do a bitmap implementation
//! copy and paste from prof's code

//level of node idx
int levelIdx(size_t idx){
  return (int)floor(log2(idx));
};
//index of the buddy with idx 
int buddyIdx(int idx){ 
   if (idx&0x1){
    return idx-1;
  }
  return idx+1;
}

int parentIdx(int idx){
    return idx/2;
}

int startIdx(int idx){
  return (idx -(1<<levelIdx(idx)));
}




void bitmap_print(BitMap *bit_map){
    int tot = levelIdx(bit_map->num_bits) - 1;  //numero di livelli totale
    int bit_left = 0; //bit rimasti da stampare per ogni lev
    int lev = -1; 
    for (int i = 0; i < bit_map->num_bits; i++){  
        if (bit_left == 0){ //lev terminato
            if(lev==tot){ 
              break; //finito 
            } 
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


void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  //! DA IMPLEMENTAREs
}


//releases allocated memory
void BuddyAllocator_free(BuddyAllocator *alloc, void *mem){
  //! DA IMPLEMENTARE
    }




