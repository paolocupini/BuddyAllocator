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
  return (idx-(1<<levelIdx(idx)));
}

int BuddyAllocator_init(BuddyAllocator* alloc,
                        int num_levels,
                        char *buffer,
                        int buffer_size,
                        int min_bucket_size
){
  //! To do: add control on sizes

  alloc->num_levels=num_levels;
  alloc->buffer=buffer;
  alloc->buffer_size=buffer_size;
  alloc->min_bucket_size=min_bucket_size;


return 1;

}


void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
    //! TO DO
    return NULL;
};


//releases allocated memory
void BuddyAllocator_free(BuddyAllocator *alloc, void *mem){
    //! TO DO
    return NULL;
};
